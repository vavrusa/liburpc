/***************************************************************************
*   Copyright (C) 2009 Marek Vavrusa <marek@vavrusa.com>                  *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU Library General Public License as       *
*   published by the Free Software Foundation; either version 2 of the    *
*   License, or (at your option) any later version.                       *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU Library General Public     *
*   License along with this program; if not, write to the                 *
*   Free Software Foundation, Inc.,                                       *
*   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
***************************************************************************/
/*! \file socket.cpp
    \brief C++ style wrapper for BSD sockets with state checking and error control.
    \author Marek Vavrusa <marek@vavrusa.com>
    \addtogroup protocpp
    @{
  */
#include "socket.hpp"
#include <cstring>
#include <iostream>
#include <sstream>
#include <netdb.h>
#include <arpa/inet.h>

using namespace std;

Socket::Socket(int fd)
   : mSock(fd), mPort(0), mOpen(false)
{
}

Socket::~Socket()
{
}

int Socket::create()
{
   // Skip on existing socket
   if(mSock != -1) {
      return Ok;
   }

   // Create socket
   mSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

   // Reuse open socket
   int state = 1;
   if(setsockopt(mSock, SOL_SOCKET, SO_REUSEADDR, (const char*) &state, sizeof(state)) < 0)
      return -1;

   return Ok;
}

int Socket::connect(std::string host, int port)
{
   if(create() < 0)
      return IOError;

   // Save host and port
   mHost = host;
   mPort = port;

   return reconnect();
}

int Socket::reconnect()
{
   // Check host, port and socket
   if(mHost.empty() || mPort <= 0 || mSock == -1)
      return ConnectError;

   // Ignore multiple connect
   if(isOpen())
      return ConnectError;

   // Create host entity
   hostent* hent = 0;
   if((hent = gethostbyname(mHost.c_str())) == 0)
      return BadAddr;

   // Connect to host
   bzero(&mAddr, sizeof(mAddr));
   mAddr.sin_family = AF_INET;
   mAddr.sin_port = htons(mPort);
   memcpy(&mAddr.sin_addr, hent->h_addr, hent->h_length);
   if(::connect(mSock, (sockaddr*) &mAddr, sizeof(mAddr)) < 0) {
      return ConnectError;
   }

   mOpen = true;
   return Ok;
}

int Socket::send(const char* buf, size_t size)
{
   if(!isOpen())
      return NotOpen;

   int sent = 0;
   if((sent = ::send(mSock, buf, size, 0)) < 0)
      return SendError;

   return sent;
}

int Socket::listen(int port, int addr, int limit)
{
   // Create socket
   if(create() < 0)
      return IOError;

   // Bind to given port
   if(bind(port, addr) < 0)
      return IOError;

   // Listen
   if(::listen(mSock, limit) < 0)
      return IOError;

   mOpen = true;
   return Ok;
}

int Socket::accept()
{
   sockaddr_in client_addr;
   socklen_t client_addr_size;
   client_addr_size = sizeof(struct sockaddr_in);
   int client = ::accept(sock(), (sockaddr*) &client_addr, &client_addr_size);
   return client;
}

int Socket::bind(int port, int addr)
{
   // Create address
   mAddr.sin_family = AF_INET; // Addr type
   mAddr.sin_port = htons(port); // Set port
   mAddr.sin_addr.s_addr = INADDR_ANY; // First available addr

   // Localhost only
   if(addr == Local)
      mAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

   if(::bind(mSock, (sockaddr*) &mAddr, sizeof(mAddr)) < 0)
      return -1;

   mHost = "localhost";
   mPort = port;
   return Ok;
}


int Socket::close()
{
   // Close open socket
   if(isOpen()) {
      if(::close(mSock) < 0)
         return IOError;

      mSock = -1;
   }

   mOpen = false;
   return Ok;
}
/** @} */

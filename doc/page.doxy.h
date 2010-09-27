/**
    \mainpage liburpc API documentation.

    liburpc is a set of APIs for transporting function calls
    over IP networks. It features a flexible RPC framework
    based on ASN.1 encoding, with type and byte-order checking.

    <h2>Installation</h2>
    Unpack source package or clone latest git. Installation requires CMake >= 2.6, GCC.
    \code
    user@localhost# mkdir build && cd build # Create build directory
    user@localhost# cmake ..                # Create Makefiles using CMake
    user@localhost# make                    # Build
    user@localhost# sudo make install       # Install
    \endcode

    <br/>
    Marek Vavrusa <marek@vavrusa.com><br/>
*/

/* Modules. */

/** \defgroup proto Protocol definitions and C API.
  * \defgroup protocpp Protocol abstraction in C++.
  * \defgroup server Server prototype.
  */


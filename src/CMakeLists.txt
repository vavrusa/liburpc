# Includes
include_directories( ${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}
                     )

# Set library prefixes
SET(LIBDIR "lib${LIB_SUFFIX}")

# Targets
set(sources  common.c
              protobase.c
              protocol.c
              protocol.cpp
              socket.cpp
              clientsocket.cpp
              serversocket.cpp 
              cmdflags.cpp
              )
set(headers  common.h
              protobase.h
              protocol.h
              protocol.hpp
              socket.hpp
              clientsocket.hpp
              serversocket.hpp
              cmdflags.hpp
              )

# Create library
add_library(urpc SHARED ${sources} ${headers})
add_library(urpc_static STATIC ${sources} ${headers})
set_target_properties(urpc PROPERTIES VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.0 SOVERSION 1)

# Install
install( TARGETS urpc
         LIBRARY DESTINATION ${LIBDIR}
         )

install( TARGETS urpc_static
         ARCHIVE DESTINATION ${LIBDIR}
         )

install( FILES ${headers}
         DESTINATION include/${PROJECT_NAME}
         )


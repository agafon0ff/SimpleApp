set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SA_NETWORK_SOURCES
    udpsocketlinux.cpp
    udpsocketwindows.cpp
    tcpsocketlinux.cpp
    tcpsocketwindows.cpp
    tcpserverlinux.cpp
    tcpserverwindows.cpp)

set(SA_NETWORK_HEADERS
    udpsocket.h
    tcpsocket.h
    tcpserver.h)

add_library(SANetwork ${SA_NETWORK_SOURCES} ${SA_NETWORK_HEADERS})

if (WIN32)
    target_link_libraries(SANetwork PRIVATE wsock32 ws2_32)
endif (WIN32)

target_include_directories(SANetwork PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR/src/SANetwork}")

add_definitions(-DSACore)
target_include_directories(SANetwork PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR/src/SACore}")
target_link_libraries(SANetwork PRIVATE SACore)


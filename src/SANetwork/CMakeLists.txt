set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
    target_link_libraries(SA wsock32 ws2_32)
endif (WIN32)

set(SA_NETWORK_SOURCES
    udpsocketlinux.cpp
    udpsocketwindows.cpp
    tcpsocket.cpp)

set(SA_NETWORK_HEADERS
    udpsocket.h
    tcpsocket.h)


add_library(SANetwork ${SA_NETWORK_SOURCES} ${SA_NETWORK_HEADERS})

target_include_directories(SANetwork PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR/src/SANetwork}")

add_definitions(-DSACore)
target_link_libraries(SANetwork PRIVATE SACore)
target_include_directories(SANetwork PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR/src/SACore}")

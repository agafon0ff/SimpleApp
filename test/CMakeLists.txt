cmake_minimum_required(VERSION 3.5)

project(SimpleApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -no-pie")

set(SOURCES main.cpp
    snakegame.cpp
    controls.cpp
    udpsockettest.cpp
    tcpsockettest.cpp)

set(HEADERS
    snakegame.h
    controls.h
    udpsockettest.h
    tcpsockettest.h)

include_directories(test)

add_definitions(-DSACore)
add_executable(SimpleApp ${SOURCES} ${HEADERS})
target_link_libraries(SimpleApp PRIVATE SACore SAGui SANetwork)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_definitions(-D__PRETTY_FUNCTION__ -D__FUNCSIG__)
    target_link_options(SimpleApp PRIVATE "/SUBSYSTEM:WINDOWS" "/ENTRY:mainCRTStartup")
endif()


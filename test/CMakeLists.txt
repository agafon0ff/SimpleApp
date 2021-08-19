cmake_minimum_required(VERSION 3.5)

project(SimpleApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES main.cpp snakegame.cpp controls.cpp)
set(HEADERS snakegame.h controls.h)
include_directories(test)

add_executable(SimpleApp ${SOURCES} ${HEADERS})
target_link_libraries(SimpleApp PRIVATE SA)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_definitions(-D__PRETTY_FUNCTION__ -D__FUNCSIG__)
    target_link_options(SimpleApp PRIVATE "/SUBSYSTEM:WINDOWS" "/ENTRY:mainCRTStartup")
endif()

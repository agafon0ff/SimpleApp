cmake_minimum_required(VERSION 3.5)

project(SimpleApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -no-pie") # executable file

set(SOURCES test/main.cpp
    test/snakegame.cpp
    test/texttest.cpp
    src/SACore/application.cpp
    src/SACore/object.cpp
    src/SAGui/widget.cpp
    src/SAGui/widgetlinux.cpp
    src/SAGui/widgetwindows.cpp)

set(HEADERS test/snakegame.h
    test/texttest.h
    src/SACore/application.h
    src/SACore/global.h
    src/SACore/object.h
    src/SAGui/widget.h
    src/SAGui/widgetlinux.h
    src/SAGui/widgetwindows.h)

include_directories(src
    src/SACore
    src/SAGui
    test)

if (UNIX)
    # X11 libs
    # sudo apt install libxtst-dev -y
    find_package(X11 REQUIRED)
    link_libraries(${X11_LIBRARIES})
    include_directories(${X11_INCLUDE_DIR})

    message(STATUS "X11_FOUND = ${X11_FOUND}")
    message(STATUS "X11_INCLUDE_DIR = ${X11_INCLUDE_DIR}")
    message(STATUS "X11_LIBRARIES = ${X11_LIBRARIES}")

endif (UNIX)

if (WIN32)
    # user32.lib
    find_library(USER32 user32)

endif (WIN32)

add_executable(SimpleApp ${SOURCES} ${HEADERS})

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_definitions(-D__PRETTY_FUNCTION__ -D__FUNCSIG__)

    # Tell the linker that this is a GUI-application
    target_link_options(SimpleApp PRIVATE "/SUBSYSTEM:WINDOWS" "/ENTRY:mainCRTStartup")
endif()

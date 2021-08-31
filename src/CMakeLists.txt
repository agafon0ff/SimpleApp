set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -no-pie")

set(SA_SOURCES
    SACore/application.cpp
    SACore/object.cpp
    SAGui/widget.cpp
    SAGui/widgetlinux.cpp
    SAGui/widgetwindows.cpp
    SAGui/button.cpp)

set(SA_HEADERS
    SACore/application.h
    SACore/global.h
    SACore/object.h
    SACore/structs.h
    SAGui/widget.h
    SAGui/widgetlinux.h
    SAGui/widgetwindows.h
    SAGui/button.h)

set(SA_DIRS
    SACore
    SAGui)

if (UNIX)
    # X11 libs
    # sudo apt install libxtst-dev -y
    find_package(X11 REQUIRED)
    link_libraries(${X11_LIBRARIES})
    include_directories(${X11_INCLUDE_DIR})
endif (UNIX)

if (WIN32)
    # user32.lib
    find_library(USER32 user32)
endif (WIN32)

add_library(SA ${SA_SOURCES} ${SA_HEADERS})

target_include_directories(SA PUBLIC ${SA_DIRS}
    "${CMAKE_CURRENT_SOURCE_DIR/src}"
    "${CMAKE_CURRENT_SOURCE_DIR/src/SACore}"
    "${CMAKE_CURRENT_SOURCE_DIR/src/SAGui}")

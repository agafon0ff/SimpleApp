set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

set(SA_GUI_SOURCES
    widget.cpp
    widgetlinux.cpp
    widgetwindows.cpp
    button.cpp
    textedit.cpp
    label.cpp
    lineedit.cpp
    scrollbar.cpp
    clipboard.cpp
    checkbox.cpp)

set(SA_GUI_HEADERS
    widget.h
    widgetlinux.h
    widgetwindows.h
    button.h
    textedit.h
    label.h
    lineedit.h
    scrollbar.h
    clipboard.h
    checkbox.h)

add_library(SAGui ${SA_GUI_SOURCES} ${SA_GUI_HEADERS})

target_link_libraries(SAGui PRIVATE SACore)
target_include_directories(SAGui PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR/src/SACore}"
    "${CMAKE_CURRENT_SOURCE_DIR/src/SAGui}")

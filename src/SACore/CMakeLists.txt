set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SA_CORE_SOURCES
    application.cpp
    object.cpp)

set(SA_CORE_HEADERS
    application.h
    global.h
    object.h
    structs.h
    utility.h)

add_library(SACore ${SA_CORE_SOURCES} ${SA_CORE_HEADERS})

target_include_directories(SACore PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR/src/SACore}")

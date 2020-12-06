cmake_minimum_required(VERSION 3.0)

project(sdptransform VERSION 1.0.0)


# For CMake >= 3.1.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# For CMake < 3.1.
add_compile_options(-std=c++11)

include_directories(${sdptransform_SOURCE_DIR}/)

set(
        SOURCE_FILES
        src/grammar.cpp
        src/parser.cpp
        src/writer.cpp
        src/sdpinfo.cpp

)

set(
        HEADER_FILES
        include/sdptransform.h
        include/json.hpp
)

add_library(sdptransform STATIC ${SOURCE_FILES} ${HEADER_FILES})
set_target_properties(sdptransform PROPERTIES LINK_FLAGS_RELEASE -s)


set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall")

install(TARGETS sdptransform DESTINATION ./lib)
install(FILES ${HEADER_FILES} DESTINATION ./include)



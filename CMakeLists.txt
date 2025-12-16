cmake_minimum_required(VERSION 3.12)

project(sciche)

file(GLOB LAX_SOURCES src/*.cpp src/byte-code/*.cpp src/common/*.cpp)
find_package(Armadillo REQUIRED)
add_executable(sciche ${LAX_SOURCES})
target_include_directories(sciche PRIVATE src src/byte-code src/common gen)
set_target_properties(sciche PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS OFF)
target_compile_options(sciche PRIVATE -Werror -Wall -Wextra)
target_compile_definitions(sciche PRIVATE $<$<CONFIG:Debug>:LAX_GC_DEBUG=1;LAX_DEBUG=1>)
target_link_libraries(sciche readline armadillo)

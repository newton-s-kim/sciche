cmake_minimum_required(VERSION 3.12)

file(GLOB LAX_SOURCES *.cpp)
find_package(Armadillo REQUIRED)
add_executable(sciche ${LAX_SOURCES})
target_include_directories(sciche PRIVATE . ../byte-code/src ../nsl)
set_target_properties(sciche PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS OFF)
target_compile_options(sciche PRIVATE -Werror -Wall -Wextra)
#target_compile_definitions(sciche PRIVATE $<$<CONFIG:Debug>:LAX_GC_DEBUG=1;LAX_DEBUG=1>)
target_link_libraries(sciche scvm readline armadillo)

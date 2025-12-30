cmake_minimum_required(VERSION 3.12)

file(GLOB MATH_SOURCES *.cpp)
find_package(Armadillo REQUIRED)
add_library(math SHARED ${MATH_SOURCES})
target_include_directories(math PRIVATE . ../../sciche/byte-code)
set_target_properties(math PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS OFF)
target_compile_options(math PRIVATE -Werror -Wall -Wextra)
target_compile_definitions(math PRIVATE $<$<CONFIG:Debug>:LAX_GC_DEBUG=1;LAX_DEBUG=1>)
target_link_libraries(math armadillo)

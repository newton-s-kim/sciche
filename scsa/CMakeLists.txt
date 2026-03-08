cmake_minimum_required(VERSION 3.12)

file(GLOB LAX_SOURCES ../sciche/*.cpp ../byte-code/src/*.cpp ../byte-code/src/objects/*.cpp)
find_package(Armadillo REQUIRED)
add_executable(scsa ${LAX_SOURCES})
target_include_directories(scsa PRIVATE . ../sciche ../byte-code/src ../nsl)
set_target_properties(scsa PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS OFF)
target_compile_options(scsa PRIVATE -Werror -Wall -Wextra)
#target_compile_definitions(scsa PRIVATE $<$<CONFIG:Debug>:LAX_GC_DEBUG=1;LAX_DEBUG=1>)
target_link_libraries(scsa readline armadillo)

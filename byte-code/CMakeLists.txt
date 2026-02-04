cmake_minimum_required(VERSION 3.12)

file(GLOB SCICHE_SOURCES src/*.cpp)
find_package(Armadillo REQUIRED)
add_library(scvm SHARED ${SCICHE_SOURCES})
target_include_directories(scvm PRIVATE src)
set_target_properties(scvm PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS OFF)
target_compile_options(scvm PRIVATE -Werror -Wall -Wextra)
#target_compile_definitions(scvm PRIVATE $<$<CONFIG:Debug>:LAX_GC_DEBUG=1;LAX_DEBUG=1>)
target_link_libraries(scvm armadillo)

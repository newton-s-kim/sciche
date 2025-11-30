cmake_minimum_required(VERSION 3.12)

project(loch)

file(GLOB LAX_SOURCES src/*.cpp src/byte-code/*.cpp src/common/*.cpp)
add_executable(loch ${LAX_SOURCES})
target_include_directories(loch PRIVATE src src/byte-code src/common gen)
set_target_properties(loch PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS OFF)
target_compile_options(loch PRIVATE -Werror -Wall -Wextra)
target_compile_definitions(loch PRIVATE $<$<CONFIG:Debug>:LAX_GC_DEBUG=1;LAX_DEBUG=1>)
target_link_libraries(loch readline)

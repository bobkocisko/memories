cmake_minimum_required(VERSION 3.6)
project(memories)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(src)

add_executable(memories_main_exe exe_main/main.cpp $<TARGET_OBJECTS:memories_compiled_source>)

add_executable(memories_test_exe exe_test/main.cpp $<TARGET_OBJECTS:memories_compiled_source>)



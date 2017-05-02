cmake_minimum_required(VERSION 3.6)
project(memories)

set(CMAKE_CXX_STANDARD 14)

find_package(X11 REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(src)

add_executable(memories_main_exe exe_main/main.cpp $<TARGET_OBJECTS:memories_compiled_source>)

add_executable(memories_test_exe exe_test/main.cpp $<TARGET_OBJECTS:memories_compiled_source>)


target_link_libraries(memories_main_exe PRIVATE ${X11_X11_LIB})
target_link_libraries(memories_main_exe PRIVATE ${OPENGL_LIBRARIES} dl)
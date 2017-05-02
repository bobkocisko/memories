set(THIRD_PARTY_SOURCE_FILES ../third_party/src/gl3w.c)

set(SOURCE_FILES
        system/ApplicationMain.cpp system/_/DispatchEventsLinux.cpp system/_/EventSourceLinux.cpp
        system/_/CreateWindowLinux.cpp)

add_library(memories_compiled_source OBJECT ${THIRD_PARTY_SOURCE_FILES} ${SOURCE_FILES})
target_include_directories(memories_compiled_source PUBLIC ../include ../third_party/include)
target_include_directories(memories_compiled_source PUBLIC ${OPENGL_INCLUDE_DIR})
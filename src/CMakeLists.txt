set(SOURCE_FILES
        system/ApplicationMain.cpp system/_/DispatchEventsLinux.cpp system/EventSourceLinux.cpp)

add_library(memories_compiled_source OBJECT ${SOURCE_FILES})
target_include_directories(memories_compiled_source PUBLIC ../include)

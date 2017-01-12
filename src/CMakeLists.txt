set(SOURCE_FILES
        ApplicationMain.cpp)

add_library(memories_compiled_source OBJECT ${SOURCE_FILES})
target_include_directories(memories_compiled_source PUBLIC ../include)

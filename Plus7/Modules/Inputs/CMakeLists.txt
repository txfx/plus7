file(GLOB_RECURSE SOURCE_FILES Src/*.c Src/*.cpp Src/*.hpp)
add_library(Inputs STATIC ${SOURCE_FILES})

target_include_directories(Inputs PUBLIC Include)
target_link_libraries(Inputs PRIVATE SDL2 Core)
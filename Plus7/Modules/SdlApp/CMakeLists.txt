file(GLOB_RECURSE SOURCE_FILES Src/*.cpp Src/*.hpp)
add_library(SdlApp STATIC ${SOURCE_FILES})

target_include_directories(SdlApp PUBLIC Include)
target_link_libraries(SdlApp PRIVATE SDL2 Core)
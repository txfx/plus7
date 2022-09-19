file(GLOB_RECURSE SOURCE_FILES Src/*.cpp Src/*.hpp)
add_library(Core STATIC ${SOURCE_FILES})
target_compile_features(Core PUBLIC cxx_std_17)

target_include_directories(Core PUBLIC Include)
target_link_libraries(Core PUBLIC rx-ranges)
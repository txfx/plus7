file(GLOB_RECURSE SOURCE_FILES Src/*.c Src/*.cpp Src/*.hpp)
add_library(ImGuiModule STATIC ${SOURCE_FILES})

target_include_directories(ImGuiModule PUBLIC Include)
target_link_libraries(ImGuiModule PUBLIC Imgui Inputs Core Gfx)

# TODO remove
add_compile_definitions(IMGUI_MODULE_NEED_SDL=1)
target_link_libraries(ImGuiModule PRIVATE SDL2)
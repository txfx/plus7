set(IMGUI_DIR ../Libs/ImGui-1.65)
file(GLOB IMGUI_SOURCE_FILES ${IMGUI_DIR}/*.cpp)
add_library(Imgui STATIC ${IMGUI_SOURCE_FILES})

file(GLOB_RECURSE SOURCE_FILES *.cpp *.hpp)
add_library(Plus7 STATIC ${SOURCE_FILES})
target_compile_features(Plus7 PUBLIC cxx_std_17)
target_compile_options(Plus7 PRIVATE -Wall -Wpedantic -Wextra -Werror -Wnon-virtual-dtor -Wshadow -Wunused -Woverloaded-virtual -Wmisleading-indentation -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference -Wuseless-cast -Wdouble-promotion)

set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR} ${IMGUI_DIR})
target_link_libraries(Plus7 PUBLIC SDL2::SDL2 ${OPENGL_LIBRARIES} Imgui)

target_include_directories(Plus7 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${OPENGL_INCLUDE_DIR} ${IMGUI_DIR})
file(GLOB_RECURSE SOURCE_FILES Src/*.c Src/*.cpp Src/*.hpp)
add_library(Gfx STATIC ${SOURCE_FILES})

set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)

target_link_libraries(Gfx PRIVATE Core SdlApp SDL2 ${OPENGL_LIBRARIES})
target_include_directories(Gfx PUBLIC Include)
set(SOURCE_FILES
    App.cpp
    Modules/ImGui.cpp
    Modules/Gfx/Dummy/DummyRenderer.cpp
    Modules/Gfx/SdlGl/SdlGlRenderer.cpp
    Modules/Gfx/SdlGl/GlBlendState.cpp
    Modules/Gfx/SdlGl/GlBuffer.cpp
    Modules/Gfx/SdlGl/GlCommandBuffer.cpp
    Modules/Gfx/SdlGl/GlDepthState.cpp
    Modules/Gfx/SdlGl/GlRasterizerState.cpp
    Modules/Gfx/SdlGl/GlShaderState.cpp
    Modules/Gfx/SdlGl/GlTexture.cpp
    Modules/Gfx/SdlGl/GlVertexLayout.cpp
    Tasks/Pipeline.cpp)

set(IMGUI_DIR ../Libs/ImGui-1.52)
set(IMGUI_SOURCE_FILES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp)

add_library(Plus7 STATIC ${SOURCE_FILES} ${IMGUI_SOURCE_FILES})

set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR} ${IMGUI_DIR})
target_link_libraries(Plus7 PUBLIC SDL2::SDL2 ${OPENGL_LIBRARIES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -Werror")

target_include_directories(Plus7 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${OPENGL_INCLUDE_DIR} ${IMGUI_DIR})
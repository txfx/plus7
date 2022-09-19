#pragma once

#include <BlendState.hpp>
#include <DepthState.hpp>
#include <RasterizerState.hpp>
#include <Renderer.hpp>
#include <ShaderState.hpp>
#include <Tasks/Task.hpp>
#include <Texture.hpp>
#include <VertexLayout.hpp>

struct ImDrawData;
struct ImGuiContext;

namespace p7::inputs {
struct MouseState;
struct KeyboardState;
} // namespace p7::inputs

namespace p7::gfx {

struct ImGui
{
public:
    ImGui(tasks::Pipeline&                      _pipeline,
          tasks::TypedID<inputs::MouseState>    _mouseTask,
          tasks::TypedID<inputs::KeyboardState> _keyboardTask,
          Renderer&                             _renderer);
    ~ImGui();

    // Tasks
    const tasks::TypedID<uint64_t> beginFrameTask;
    const tasks::TypedID<void>     endFrameTask;

private:
    uint64_t BeginFrame(const inputs::MouseState&    _mouseState,
                        const inputs::KeyboardState& _keyboardState,
                        const Renderer&              _renderer);
    void     EndFrame(Renderer& _renderer);

    void DrawLists(ImDrawData* draw_data, Renderer& _renderer);

private:
    // render states
    BlendState      blendState;
    DepthState      depthState;
    RasterizerState rasterizerState;

    // shader
    ShaderState shader;

    // Vertex layout
    VertexLayout vtxLayout;

    // Font
    TexturePtr font;

    // ImGui
    ImGuiContext* context;

    uint64_t frame    = 0;
    uint32_t lastTick = 0;
};
} // namespace p7::gfx
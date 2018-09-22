#pragma once

#include "Module.hpp"

#include <Modules/Gfx/Renderer.hpp>
#include <Modules/Inputs/Inputs.hpp>
#include <Tasks/Task.hpp>

struct ImDrawData;
struct ImGuiContext;

namespace p7::gfx {

struct ImGui : public Module
{
public:
    explicit ImGui(App& _app);
    ~ImGui() override;

    tasks::ID<uint64_t> GetBeginFrameTask() const { return beginFrameTask; }
    tasks::ID<void>     GetEndFrameTask() const { return endFrameTask; }

private:
    uint64_t BeginFrame(const inputs::MouseState& mouseState, const inputs::KeyboardState& keyboardState);
    void     EndFrame();

    void DrawLists(ImDrawData* draw_data);

private:
    // Modules dependencies
    inputs::Mouse&    mouse;
    inputs::Keyboard& keyboard;
    Renderer&         renderer;

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

    // Tasks
    tasks::ID<uint64_t> beginFrameTask;
    tasks::ID<void>     endFrameTask;

    // ImGui
    ImGuiContext* context;

    uint64_t frame = 0;
};
} // namespace p7::gfx
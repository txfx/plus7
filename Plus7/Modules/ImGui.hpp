#pragma once

#include "App.hpp"

#include <Modules/Gfx/Renderer.hpp>
#include <Modules/Inputs/Inputs.hpp>
#include <Tasks/Task.hpp>

struct ImDrawData;
struct ImGuiContext;

namespace p7::gfx {

struct ImGui : public ModuleWithDependencies<inputs::Mouse, inputs::Keyboard, Renderer>
{
public:
    explicit ImGui(App& _app);
    ~ImGui() override;

    auto GetBeginFrameTask() const { return beginFrameTask; }
    auto GetEndFrameTask() const { return endFrameTask; }

private:
    uint64_t BeginFrame(const inputs::MouseState& mouseState, const inputs::KeyboardState& keyboardState);
    void     EndFrame();

    void DrawLists(ImDrawData* draw_data);

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

    // Tasks
    tasks::TypedID<uint64_t> beginFrameTask;
    tasks::TypedID<void>     endFrameTask;

    // ImGui
    ImGuiContext* context;

    uint64_t frame = 0;
};
} // namespace p7::gfx
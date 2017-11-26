#pragma once

#include "Module.hpp"

#include <Modules/Gfx/Renderer.hpp>
#include <Tasks/Task.hpp>

struct ImDrawData;

namespace p7 {
namespace gfx {

struct ImGui : public Module
{
public:
    explicit ImGui(App& _app);

    tasks::ID GetBeginFrameTask() const { return beginFrameTask; }
    tasks::ID GetEndFrameTask() const { return endFrameTask; }

private:
    void BeginFrame();
    void EndFrame();

    void DrawLists(ImDrawData* draw_data);

private:
    Renderer& renderer;

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
    tasks::ID beginFrameTask;
    tasks::ID endFrameTask;
};
} // namespace gfx
} // namespace p7
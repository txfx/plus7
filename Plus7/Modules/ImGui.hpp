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

    tasks::ID<uint64_t> GetBeginFrameTask() const { return beginFrameTask; }
    tasks::ID<void>     GetEndFrameTask() const { return endFrameTask; }

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
    tasks::ID<uint64_t> beginFrameTask;
    tasks::ID<void>     endFrameTask;
};
} // namespace gfx
} // namespace p7
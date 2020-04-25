#pragma once

#include <Tasks/Task.hpp>

#include "CommandBuffer.hpp"
#include "Dummy/DummyRenderer.hpp"

struct SDL_Window;
using SDL_GLContext = void*;

namespace p7::tasks {
struct Pipeline;
} // namespace p7::tasks

namespace p7::gfx::sdl {

struct SdlGlRenderer final : DummyRenderer
{
public:
    SdlGlRenderer(const char* _name, int _w, int _h, bool _visible, tasks::Pipeline& _pipeline);
    ~SdlGlRenderer() override;

    CommandBuffer& GetCommandBuffer() override { return commandBuffer; }

    uint32_t GetWidth() const override { return width; }
    uint32_t GetHeight() const override { return height; }

    tasks::TypedID<void> GetEndFrameTask() const override { return endFrameTask; }

private:
    void EndFrame();

private:
    const tasks::TypedID<void> endFrameTask;
    int                        width;
    int                        height;
    bool                       initialized;
    SDL_Window*                window;
    SDL_GLContext              glcontext;
    GlCommandBuffer            commandBuffer;
};

} // namespace p7::gfx::sdl
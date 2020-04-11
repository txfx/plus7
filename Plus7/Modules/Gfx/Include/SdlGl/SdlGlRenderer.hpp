#pragma once

#include <Module.hpp>
#include <Tasks/Task.hpp>

#include "CommandBuffer.hpp"
#include "Dummy/DummyRenderer.hpp"

struct SDL_Window;
using SDL_GLContext = void*;

namespace p7 {

struct App;
struct SdlApp;

namespace gfx {

struct SdlGlRenderer final : public ModuleWithDependencies<SdlApp>, public DummyRenderer
{
public:
    explicit SdlGlRenderer(App& _app);
    SdlGlRenderer(const char* _name, int _w, int _h, App& _app);
    ~SdlGlRenderer() override;

    inline CommandBuffer& GetCommandBuffer() { return commandBuffer; }

    uint32_t GetWidth() const override { return width; }
    uint32_t GetHeight() const override { return height; }

    const tasks::TypedID<void> displayTask;

private:
    SdlGlRenderer(const char* _name, int _w, int _h, bool _visible, App& _app);
    void EndFrame();

private:
    int             width;
    int             height;
    bool            initialized;
    SDL_Window*     window;
    SDL_GLContext   glcontext;
    GlCommandBuffer commandBuffer;
};
} // namespace gfx
} // namespace p7
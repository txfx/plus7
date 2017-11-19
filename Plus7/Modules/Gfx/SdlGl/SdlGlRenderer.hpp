#pragma once

#include <Module.hpp>

#include <Modules/Gfx/CommandBuffer.hpp>
#include <Modules/Gfx/DummyRenderer.hpp>

struct SDL_Window;
using SDL_GLContext = void*;

namespace p7 {
namespace gfx {

struct SdlGlRenderer final : public Module, public DummyRenderer
{
public:
    explicit SdlGlRenderer(App& _app);
    SdlGlRenderer(const char* _name, int _w, int _h, App& _app);
    ~SdlGlRenderer() override;

    void Update() override;

    inline CommandBuffer& GetCommandBuffer() { return commandBuffer; }

private:
    SdlGlRenderer(const char* _name, int _w, int _h, bool _visible, App& _app);

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
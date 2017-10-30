#pragma once

#include <Module.hpp>
#include <Modules/Gfx/RasterizerProperties.hpp>

struct SDL_Window;
using SDL_GLContext = void*;

namespace p7 {
namespace gfx {

struct RasterizerState;

struct SdlGlRenderer : public Module
{
public:
    explicit SdlGlRenderer(App& _app);
    SdlGlRenderer(const char* _name, int _w, int _h, App& _app);
    ~SdlGlRenderer() override;

    // Module
    void Update() override;

private:
    SdlGlRenderer(const char* _name, int _w, int _h, bool _visible, App& _app);

private:
    int           width;
    int           height;
    bool          initialized;
    SDL_Window*   window;
    SDL_GLContext glcontext;
};
} // namespace gfx
} // namespace p7
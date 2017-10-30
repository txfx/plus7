#pragma once

#include <Module.hpp>
#include <Modules/Gfx/RasterizerProperties.hpp>
#include <Modules/Gfx/RasterizerState.hpp>

struct SDL_Window;
using SDL_GLContext = void*;

namespace p7 {
namespace gfx {

struct SdlGlRenderer : public Module
{
public:
    explicit SdlGlRenderer(App& _app);
    SdlGlRenderer(const char* _name, int _w, int _h, App& _app);
    ~SdlGlRenderer() override;

    // Module
    void Update() override;

    // Renderer
    void Clear(float _r, float _g, float _b, float _a = 1.0f);
    void SetRasterizerState(const RasterizerState& _state);

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
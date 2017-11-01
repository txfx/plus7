#pragma once

#include <Module.hpp>
#include <Modules/Gfx/RendererInterface.hpp>

struct SDL_Window;
using SDL_GLContext = void*;

namespace p7 {
namespace gfx {

struct SdlGlRenderer final : public Module, public RendererInterface
{
public:
    explicit SdlGlRenderer(App& _app);
    SdlGlRenderer(const char* _name, int _w, int _h, App& _app);
    ~SdlGlRenderer() override;

    // Module
    void Update() override;

    // Renderer
    void Clear(float _r, float _g, float _b, float _a) override;

    void SetViewport(float _x, float _y, float _w, float _h) override;
    void SetScissor(float _x, float _y, float _w, float _h) override;

    void BindIndexBuffer(Buffer& _buffer) override;
    void BindVertexBuffer(Buffer& _buffer, uint8_t _binding) override;
    void BindConstantBuffer(Buffer& _buffer, uint8_t _binding) override;

    void BindBlendState(const BlendState& _state) override;
    void BindDepthState(const DepthState& _state) override;
    void BindRasterizerState(const RasterizerState& _state) override;

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
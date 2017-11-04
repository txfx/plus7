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

    void BindIndexBuffer(const Buffer& _buffer, uint32_t _offset, uint32_t _size) override;
    using RendererInterface::BindIndexBuffer;
    void BindVertexBuffer(const Buffer& _buffer, uint32_t _offset, uint32_t _size, uint8_t _binding) override;
    using RendererInterface::BindVertexBuffer;
    void BindConstantBuffer(const Buffer& _buffer, uint32_t _offset, uint32_t _size, uint8_t _binding) override;
    using RendererInterface::BindConstantBuffer;

    void BindBlendState(const BlendState& _state) override;
    void BindDepthState(const DepthState& _state) override;
    void BindRasterizerState(const RasterizerState& _state) override;

    void BindShaderState(const ShaderState& _state) override;

    void BindTexture(const Texture& _texture, uint8_t _binding) override;

    void Draw(uint32_t _vertexCount, uint32_t _instanceCount, uint32_t _firstVertex, uint32_t _firstInstance) override;
    using RendererInterface::Draw;
    void DrawIndexed(uint32_t _indexCount, uint32_t _instanceCount, uint32_t _firstIndex, int32_t _vertexOffset, uint32_t _firstInstance) override;
    using RendererInterface::DrawIndexed;

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
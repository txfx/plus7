#include "SdlGlRenderer.hpp"

#include <SDL.h>

#include "gl_headers.h"

namespace p7 {
namespace gfx {

SdlGlRenderer::SdlGlRenderer(App& _app)
    : SdlGlRenderer("", 0, 0, /* _visible*/ false, _app)
{}

SdlGlRenderer::SdlGlRenderer(const char* _name, int _w, int _h, App& _app)
    : SdlGlRenderer(_name, _w, _h, /*_visible*/ true, _app)
{}

SdlGlRenderer::SdlGlRenderer(const char* _name, int _w, int _h, bool _visible, App& _app)
    : Module(_app)
    , width(_w)
    , height(_h)
{
    initialized = SDL_InitSubSystem(SDL_INIT_VIDEO) == 0;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    uint32_t flags = SDL_WINDOW_OPENGL;
    flags |= _visible ? SDL_WINDOW_SHOWN : SDL_WINDOW_HIDDEN;
    //Create window
    window    = SDL_CreateWindow(_name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);
    glcontext = SDL_GL_CreateContext(window);
}

SdlGlRenderer::~SdlGlRenderer()
{
    if (initialized)
    {
        SDL_GL_DeleteContext(glcontext);
        SDL_DestroyWindow(window);
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
    }
}

void SdlGlRenderer::Clear(float _r, float _g, float _b, float _a)
{
    glClearColor(_r, _g, _b, _a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void SdlGlRenderer::SetViewport(float /* _x */, float /* _y */, float /* _w */, float /* _h */) {}
void SdlGlRenderer::SetScissor(float /* _x */, float /* _y */, float /* _w */, float /* _h */) {}
void SdlGlRenderer::BindIndexBuffer(const Buffer& /* _buffer */, uint32_t /* _offset */, uint32_t /* _size */) {}
void SdlGlRenderer::BindVertexBuffer(const Buffer& /* _buffer */, uint32_t /* _offset */, uint32_t /* _size */, uint8_t /* _binding */) {}
void SdlGlRenderer::BindConstantBuffer(const Buffer& /* _buffer */, uint32_t /* _offset */, uint32_t /* _size */, uint8_t /* _binding */) {}

void SdlGlRenderer::BindBlendState(const BlendState& _state)
{
    _state.Apply();
}

void SdlGlRenderer::BindDepthState(const DepthState& _state)
{
    _state.Apply();
}

void SdlGlRenderer::BindRasterizerState(const RasterizerState& _state)
{
    _state.Apply();
}

void SdlGlRenderer::BindShaderState(const ShaderState& _state)
{
    _state.Apply();
}

void SdlGlRenderer::BindTexture(const Texture& /* _texture */, uint8_t /* _binding */) {}

void SdlGlRenderer::Draw(
    uint32_t /* _vertexCount */,
    uint32_t /* _instanceCount */,
    uint32_t /* _firstVertex */,
    uint32_t /* _firstInstance */)
{}

void SdlGlRenderer::DrawIndexed(
    uint32_t /* _indexCount */,
    uint32_t /* _instanceCount */,
    uint32_t /* _firstIndex */,
    int32_t /* _vertexOffset */,
    uint32_t /* _firstInstance */)
{}

void SdlGlRenderer::Update()
{
    glViewport(0, 0, width, height);
    Clear(1, 0, 0, 1);
    SDL_GL_SwapWindow(window);
}

} // namespace gfx
} // namespace p7
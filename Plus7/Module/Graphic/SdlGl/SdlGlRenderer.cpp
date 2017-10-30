#include "SdlGlRenderer.hpp"

#include <GL/gl.h>
#include <SDL.h>

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

void SdlGlRenderer::Update()
{
    glViewport(0, 0, width, height);
    glClearColor(1, 1, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(window);
}

} // namespace gfx
} // namespace p7
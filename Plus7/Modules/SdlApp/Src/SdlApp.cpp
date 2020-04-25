#include "SdlApp.hpp"

#include <SDL.h>
#include <Sdl/SdlKeyboard.hpp>
#include <Sdl/SdlMouse.hpp>
#include <SdlGl/SdlGlRenderer.hpp>
#include <Tasks/Pipeline.hpp>

namespace {
bool PumpEvents()
{
    SDL_PumpEvents();

    SDL_Event event;
    return SDL_PeepEvents(&event, 1, SDL_eventaction::SDL_GETEVENT, SDL_QUIT, SDL_QUIT) == 1;
}
} // namespace

namespace p7::sdl {

using namespace tasks;

WindowedApp::WindowedApp(Pipeline& _pipeline, const char* _name, int _w, int _h)
    : mainTask { _pipeline.AddTask("SDL events"_name, PumpEvents) }
    , mouseTask { inputs::sdl::CreateUpdateMouseTask(_pipeline, mainTask) }
    , keyboardTask { inputs::sdl::CreateUpdateKeyboardTask(_pipeline, mainTask) }
    , renderer { std::make_unique<gfx::sdl::SdlGlRenderer>(_name, _w, _h, true, _pipeline) }
{
    SDL_Init(SDL_INIT_EVENTS | SDL_INIT_TIMER);
}

WindowedApp::~WindowedApp()
{
    SDL_Quit();
}

} // namespace p7::sdl

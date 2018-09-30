#include "SdlApp.hpp"

#include <App.hpp>
#include <SDL.h>

namespace p7 {

using namespace tasks;

SdlApp::SdlApp(App& _app)
    : Module(_app)
    , mainTask(_app.CreateTask(
          "SDL events"_name,
          [&]() { this->PollEvents(); }))
{
    SDL_Init(SDL_INIT_EVENTS | SDL_INIT_TIMER);
}

SdlApp::~SdlApp()
{
    SDL_Quit();
}

void SdlApp::PollEvents()
{
    SDL_PumpEvents();

    SDL_Event event;
    if (SDL_PeepEvents(&event, 1, SDL_eventaction::SDL_GETEVENT, SDL_QUIT, SDL_QUIT) == 1)
    {
        app.RequestExit();
    }
}

} // namespace p7
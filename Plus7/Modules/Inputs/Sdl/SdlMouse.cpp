#include "SdlMouse.hpp"

#include <App.hpp>
#include <Modules/Sdl/SdlApp.hpp>
#include <SDL.h>

#include <array>

namespace p7::inputs {
SdlMouse::SdlMouse(App& _app)
    : Module(_app)
    , getStateTask(_app.CreateTask(
          [&]() { return this->PollEvents(); },
          tasks::after(app.GetDependency<SdlApp>().GetMainTask())))
{
}

MouseState SdlMouse::PollEvents()
{
    MouseState state;
    SDL_Event  event;

    if (SDL_PeepEvents(&event, 1, SDL_eventaction::SDL_GETEVENT, SDL_MOUSEWHEEL, SDL_MOUSEWHEEL) == 1)
    {
        state.wheel.horizontal = event.wheel.x;
        state.wheel.vertical   = event.wheel.y;
    }

    int    mx, my;
    Uint32 mouseMask = SDL_GetMouseState(&mx, &my);

    state.pos.x      = (float)mx;
    state.pos.y      = (float)my;
    state.buttons[0] = (mouseMask & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;
    state.buttons[1] = (mouseMask & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0;
    state.buttons[2] = (mouseMask & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0;

    return state;
}

} // namespace p7::inputs
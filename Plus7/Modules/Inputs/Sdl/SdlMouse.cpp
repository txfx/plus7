#include "SdlMouse.hpp"

#include <App.hpp>
#include <Modules/Sdl/SdlApp.hpp>
#include <SDL.h>

#include <array>

namespace p7::inputs {

using namespace p7::tasks;

SdlMouse::SdlMouse(App& _app)
    : ModuleWithDependencies(_app)
    , updateTask(_app.AddTask(
          "SDL mouse"_name,
          [&]() { return this->PollEvents(); },
          needs(GetModule<SdlApp>().mainTask)))
{}

MouseState SdlMouse::PollEvents()
{
    MouseState state;

    SDL_Event event;
    while (SDL_PeepEvents(&event, 1,
               SDL_eventaction::SDL_GETEVENT,
               SDL_EventType::SDL_MOUSEMOTION,
               SDL_EventType::SDL_MOUSEWHEEL)
        == 1)
    {
        switch (event.type)
        {
        case SDL_EventType::SDL_MOUSEWHEEL:
            state.wheel.horizontal = event.wheel.x;
            state.wheel.vertical   = event.wheel.y;
            break;

        case SDL_EventType::SDL_MOUSEBUTTONDOWN:
            state.buttons[0] |= event.button.button == SDL_BUTTON_LEFT;
            state.buttons[1] |= event.button.button == SDL_BUTTON_RIGHT;
            state.buttons[2] |= event.button.button == SDL_BUTTON_MIDDLE;
            break;
        }
    }

    int    mx, my;
    Uint32 mouseMask = SDL_GetMouseState(&mx, &my);

    state.pos.x = (float)mx;
    state.pos.y = (float)my;
    state.buttons[0] |= (mouseMask & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;
    state.buttons[1] |= (mouseMask & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0;
    state.buttons[2] |= (mouseMask & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0;

    return state;
}

} // namespace p7::inputs
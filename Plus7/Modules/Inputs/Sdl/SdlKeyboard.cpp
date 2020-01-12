#include "SdlKeyboard.hpp"

#include <App.hpp>
#include <Modules/Sdl/SdlApp.hpp>
#include <SDL.h>
#include <Utils/Assert.hpp>
#include <array>
#include <cstring>

namespace p7::inputs {

using namespace p7::tasks;

SdlKeyboard::SdlKeyboard(App& _app)
    : ModuleWithDependencies(_app)
    , updateTask(_app.AddTask(
          "SDL Keyboard"_name,
          needs(GetModule<SdlApp>().mainTask),
          PollEvents))
{}

KeyboardState SdlKeyboard::PollEvents()
{
    KeyboardState state;

    SDL_Event event;
    while (SDL_PeepEvents(&event, 1,
               SDL_eventaction::SDL_GETEVENT,
               SDL_EventType::SDL_KEYDOWN,
               SDL_EventType::SDL_TEXTINPUT)
        == 1)
    {
        switch (event.type)
        {
        case SDL_EventType::SDL_TEXTINPUT:
            static_assert(sizeof(event.text.text) <= sizeof(state.text));
            std::memcpy(state.text.data(), event.text.text, sizeof(event.text.text));
            break;
        }
    }

    state.shiftMod = ((SDL_GetModState() & KMOD_SHIFT) != 0);
    state.ctrlMod  = ((SDL_GetModState() & KMOD_CTRL) != 0);
    state.altMod   = ((SDL_GetModState() & KMOD_ALT) != 0);
    state.superMod = ((SDL_GetModState() & KMOD_GUI) != 0);

    int  nbKeys    = 0;
    auto keysState = SDL_GetKeyboardState(&nbKeys);
    P7_ASSERT(static_cast<uint>(nbKeys) <= state.keysDown.size(), "SDL_GetKeyboardState has returned too many keys");
    static_assert(sizeof(*keysState) == sizeof(*state.keysDown.data()));
    std::memcpy(state.keysDown.data(), keysState, nbKeys);

    return state;
}

} // namespace p7::inputs
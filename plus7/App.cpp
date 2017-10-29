#include "App.hpp"

#include <SDL.h>

namespace p7 {

App::App()
{
    initialized = SDL_Init(0) == 0;
}

App::~App()
{
    if (initialized)
    {
        SDL_Quit();
    }
}

void App::Run()
{
}

} // namespace p7
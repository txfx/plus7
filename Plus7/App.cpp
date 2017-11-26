#include "App.hpp"

#include <SDL.h>
#include <Utils/ReverseIterator.hpp>

namespace p7 {

App::App()
{
    SDL_Init(SDL_INIT_EVENTS | SDL_INIT_TIMER);
}

App::~App()
{
    modules.clear();

    SDL_Quit();
}

void App::Run()
{
    SDL_Event event{};
    while (!exit_requested)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            exit_requested |= event.type == SDL_QUIT;
        }

        for (auto& module : modules)
        {
            module->BeginFrame();
        }

        for (auto& module : modules)
        {
            module->Update();
        }

        for (auto& module : std::reverse(modules))
        {
            module->EndFrame();
        }
    }
}

void App::RequestExit()
{
    exit_requested = true;
}

} // namespace p7
#include "App.hpp"

namespace p7 {

using namespace p7::tasks;

App::App()
{}

App::~App()
{
    modules.clear();
}

void App::Run()
{
    while (!exit_requested)
    {
        ExecuteTasks();
    }
}

void App::RequestExit()
{
    exit_requested = true;
}

} // namespace p7
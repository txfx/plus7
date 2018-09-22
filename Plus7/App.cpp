#include "App.hpp"

namespace p7 {

App::App()
    : dummyTask(CreateTask([]() {}))
{
}

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
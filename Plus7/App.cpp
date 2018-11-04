#include "App.hpp"

namespace p7 {

using namespace p7::tasks;

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
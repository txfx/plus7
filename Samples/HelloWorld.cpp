#include <iostream>

#include <App.hpp>
#include <Modules/Gfx/Renderer.hpp>
#include <Modules/ImGui.hpp>

#include <imgui.h>

#include <Tasks/Task.hpp>

int main()
{
    using p7::App;
    using p7::gfx::ImGui;
    using p7::gfx::Renderer;

    App helloWorldApp;
    helloWorldApp.LoadModule<Renderer>("Hello World", 1280, 720);
    auto& imgui = helloWorldApp.LoadModule<ImGui>();

    auto test = helloWorldApp.CreateTask(
        imgui.GetBeginFrameTask(),
        []() { ::ImGui::ShowTestWindow(); },
        imgui.GetEndFrameTask());

    helloWorldApp.Run();
    std::cout << "Hello World" << std::endl;
}
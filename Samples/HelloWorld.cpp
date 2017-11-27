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

    App   helloWorldApp;
    auto& renderer = helloWorldApp.LoadModule<Renderer>("Hello World", 1280, 720);
    auto& imgui    = helloWorldApp.LoadModule<ImGui>();

    auto test = helloWorldApp.CreateTask(
        imgui.GetBeginFrameTask(),
        [&renderer]() {
            static float  f           = 0.0f;
            static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

            ::ImGui::Text("Hello, world!");
            ::ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
            ::ImGui::ColorEdit3("clear color", (float*)&clear_color);
            ::ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ::ImGui::GetIO().Framerate, ::ImGui::GetIO().Framerate);

            ::ImGui::ShowTestWindow();

            using p7::gfx::RasterizerState;
            static RasterizerState defaultState({});
            renderer.GetCommandBuffer().BindRasterizerState(defaultState);
            renderer.GetCommandBuffer().Clear(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        },
        imgui.GetEndFrameTask());

    helloWorldApp.Run();
    std::cout << "Hello World" << std::endl;
}
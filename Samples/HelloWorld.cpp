#include <iostream>

#include <ImGui.hpp>
#include <SdlApp.hpp>
#include <imgui.h>

#include <Tasks/Pipeline.hpp>

int main()
{
    using p7::gfx::ImGui;
    using p7::gfx::Renderer;
    using p7::sdl::WindowedApp;
    using namespace p7::tasks;

    Pipeline       pipeline;
    WindowedApp    app { pipeline, "Hello World", 1280, 720 };
    p7::gfx::ImGui imgui { pipeline, app.mouseTask, app.keyboardTask, *app.renderer };

    pipeline.AddTask(
        "Helloworld Sample"_name,
        consuming(imgui.beginFrameTask),
        before(imgui.endFrameTask),
        [& renderer = *app.renderer](uint64_t _frame) {
            static float  f           = 0.0f;
            static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

            ::ImGui::Text("Hello, world! %lu", _frame);
            ::ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
            ::ImGui::ColorEdit3("clear color", (float*)&clear_color);
            ::ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ::ImGui::GetIO().Framerate, ::ImGui::GetIO().Framerate);

            ::ImGui::ShowDemoWindow();

            using p7::gfx::RasterizerState;
            static RasterizerState defaultState({});
            renderer.GetCommandBuffer().BindRasterizerState(defaultState);
            renderer.GetCommandBuffer().Clear(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        });


    pipeline.Build();
    pipeline.ExecuteWhile(app.mainTask);
}
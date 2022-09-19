#include <ImGui.hpp>
#include <rx/ranges.hpp>
#include <SdlApp.hpp>
#include <Tasks/Pipeline.hpp>

#include <algorithm>
#include <fmt/format.h>
#include <functional>
#include <imgui.h>
#include <iterator>
#include <optional>

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
      "Hello world sample"_name,
      consuming(imgui.beginFrameTask),
      before(imgui.endFrameTask),
      [&renderer = *app.renderer](uint64_t _frame) {
          static float  f           = 0.0f;
          static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

          ::ImGui::Text("Hello, world! %lu", _frame);
          ::ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
          ::ImGui::ColorEdit3("clear color", (float*)&clear_color);
          ::ImGui::Text(
            "Application average %.3f ms/frame (%.1f FPS)",
            1000.0f / ::ImGui::GetIO().Framerate,
            ::ImGui::GetIO().Framerate);

          ::ImGui::ShowDemoWindow();

          using p7::gfx::RasterizerState;
          static RasterizerState defaultState({});
          renderer.GetCommandBuffer().BindRasterizerState(defaultState);
          renderer.GetCommandBuffer().Clear(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
      });

    using DepthAndId   = std::pair<int, ID>;
    using DepthsAndIds = std::vector<DepthAndId>;
    DepthsAndIds depthsAndIds;

    auto isParent = [&](ID parent, ID child) {
        auto& parents = pipeline.GetTask(child).GetParents();
        return parents | rx::any_of([parent](auto id) { return id == parent; });
    };
    auto isChild = [&](ID child, ID parent) {
        auto& children = pipeline.GetTask(parent).GetChildren();
        return children | rx::any_of([child](auto id) { return id == child; });
    };

    pipeline.AddTask("Display Task Graph"_name, after(imgui.beginFrameTask), before(imgui.endFrameTask), [&]() {
        ::ImGui::Begin("Task List");
        static std::optional<ID> hovered;
        std::optional<ID>        nextToHover;

        int last_depth = -1;
        for (auto [depth, id] : depthsAndIds)
        {
            if (last_depth == depth)
            {
                ::ImGui::SameLine();
            }
            else
            {
                last_depth = depth;
            }

            ImVec4 color = ::ImGui::GetStyleColorVec4(ImGuiCol_Button);
            if (hovered)
            {
                if (isParent(id, hovered.value()))
                {
                    color = (ImVec4)ImColor::HSV(0.5f, 0.6f, 0.6f);
                }
                else if (isChild(id, hovered.value()))
                {
                    color = (ImVec4)ImColor::HSV(0.1f, 0.6f, 0.6f);
                }
            }

            ::ImGui::PushStyleColor(ImGuiCol_Button, color);
            //::ImGui::Button(fmt::format("{}", id).c_str());
            ::ImGui::Button(pipeline.GetTask(id).GetName().value);
            ::ImGui::PopStyleColor(1);

            if (::ImGui::IsItemHovered())
            {
                nextToHover = id;
                ::ImGui::SetTooltip("%s", pipeline.GetTask(id).GetName().value);
            }
        }
        hovered = nextToHover;
        ::ImGui::End();
    });

    std::function<int(ID)> getTaskDepth;
    getTaskDepth = [&getTaskDepth, &pipeline](ID id) {
        int max = 0;
        if (id != self<void>)
        {
            for (auto pid : pipeline.GetTask(id).GetParents())
            {
                max = std::max(max, getTaskDepth(pid) + 1);
            }
        }
        return max;
    };

    auto cont = pipeline.AddTask("Check Exit"_name, consuming(app.mainTask), [](const auto& info) {
        return info.exitRequested;
    });

    auto toDepthAndId = [&](auto& t) {
        auto id = t.GetID();
        return std::make_pair(getTaskDepth(id), id);
    };

    depthsAndIds = pipeline.GetTasks() | rx::transform(toDepthAndId) | rx::sort() | rx::to_vector();

    pipeline.Build();
    pipeline.ExecuteWhile(cont);
}
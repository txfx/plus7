#include "ImGui.hpp"

#include <App.hpp>
#include <SDL.h>
#include <Utils/Assert.hpp>
#include <cstring>
#include <imgui.h>

namespace p7::gfx {

namespace {
constexpr BlendProperties      blendProps { BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha, BlendOp::Add };
constexpr DepthProperties      depthProps;
constexpr RasterizerProperties rasterizerProps { PolygonMode::Fill, CullMode::None, FrontFace::CounterClockWise, true };

constexpr const char* vertex_shader = R"(
#version 330

uniform MatrixBlock
{
    uniform mat4 ProjMtx;
};

layout(location=0) in vec4 Position_UV;
layout(location=1) in vec4 Color;

out vec2 Frag_UV;
out vec4 Frag_Color;

void main()
{
    Frag_UV = Position_UV.zw;
    Frag_Color = Color;
    gl_Position = ProjMtx * vec4(Position_UV.xy,0,1);
})";

constexpr const char* pixel_shader = R"(
#version 330

uniform sampler2D Texture;

in vec2 Frag_UV;
in vec4 Frag_Color;

out vec4 Out_Color;

void main()
{
    Out_Color = Frag_Color * texture( Texture, Frag_UV.st);
})";

constexpr VertexLayoutProperties vtxLayoutProperties = {
    { VertexAttribute::Type::XYZW32F, 0 },
    { VertexAttribute::Type::R8G8B8A8_UNORM, 0 }
};

} // namespace

using namespace p7::tasks;

ImGui::ImGui(App& _app)
    : ModuleWithDependencies(_app)
    , beginFrameTask(
          _app.CreateTask(
              "ImGui begin frame"_name,
              [&](const auto& mouseState, const auto& keyboardState) {
                  return this->BeginFrame(mouseState, keyboardState);
              },
              consume(Get<inputs::Mouse>().getStateTask, Get<inputs::Keyboard>().getStateTask)))
    , endFrameTask(
          _app.CreateTask(
              "ImGui end frame"_name,
              [&](uint64_t) { this->EndFrame(); },
              consume(beginFrameTask)
                  .run_before(Get<Renderer>().displayTask)))
    , blendState(blendProps)
    , depthState(depthProps)
    , rasterizerState(rasterizerProps)
    , shader(vertex_shader, pixel_shader)
    , vtxLayout(vtxLayoutProperties)
    , context(::ImGui::CreateContext())
{
    ImGuiIO& io = ::ImGui::GetIO();

    unsigned char* pixels;
    int            width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
    font = Get<Renderer>().CreateTexture({ static_cast<uint32_t>(width), static_cast<uint32_t>(height) }, pixels);

    io.Fonts->TexID = &font;

    static_assert(sizeof(ImDrawVert) == sizeof(ImVec2) * 2 + sizeof(ImU32));

    // Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
    io.KeyMap[ImGuiKey_Tab]        = SDL_SCANCODE_TAB;
    io.KeyMap[ImGuiKey_LeftArrow]  = SDL_SCANCODE_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow]    = SDL_SCANCODE_UP;
    io.KeyMap[ImGuiKey_DownArrow]  = SDL_SCANCODE_DOWN;
    io.KeyMap[ImGuiKey_PageUp]     = SDL_SCANCODE_PAGEUP;
    io.KeyMap[ImGuiKey_PageDown]   = SDL_SCANCODE_PAGEDOWN;
    io.KeyMap[ImGuiKey_Home]       = SDL_SCANCODE_HOME;
    io.KeyMap[ImGuiKey_End]        = SDL_SCANCODE_END;
    io.KeyMap[ImGuiKey_Insert]     = SDL_SCANCODE_INSERT;
    io.KeyMap[ImGuiKey_Delete]     = SDL_SCANCODE_DELETE;
    io.KeyMap[ImGuiKey_Backspace]  = SDL_SCANCODE_BACKSPACE;
    io.KeyMap[ImGuiKey_Space]      = SDL_SCANCODE_SPACE;
    io.KeyMap[ImGuiKey_Enter]      = SDL_SCANCODE_RETURN;
    io.KeyMap[ImGuiKey_Escape]     = SDL_SCANCODE_ESCAPE;
    io.KeyMap[ImGuiKey_A]          = SDL_SCANCODE_A;
    io.KeyMap[ImGuiKey_C]          = SDL_SCANCODE_C;
    io.KeyMap[ImGuiKey_V]          = SDL_SCANCODE_V;
    io.KeyMap[ImGuiKey_X]          = SDL_SCANCODE_X;
    io.KeyMap[ImGuiKey_Y]          = SDL_SCANCODE_Y;
    io.KeyMap[ImGuiKey_Z]          = SDL_SCANCODE_Z;
}

ImGui::~ImGui()
{
    ::ImGui::DestroyContext(context);
}

uint64_t ImGui::BeginFrame(const inputs::MouseState& mouseState, const inputs::KeyboardState& keyboardState)
{
    auto&    renderer = Get<Renderer>();
    ImGuiIO& io       = ::ImGui::GetIO();
    io.DisplaySize    = ImVec2(renderer.GetWidth(), renderer.GetHeight());

    // mouse
    io.MousePos = ImVec2(mouseState.pos.x, mouseState.pos.y);

    io.MouseDown[0] = mouseState.buttons[0];
    io.MouseDown[1] = mouseState.buttons[1];
    io.MouseDown[2] = mouseState.buttons[2];

    io.MouseWheel  = mouseState.wheel.vertical;
    io.MouseWheelH = mouseState.wheel.horizontal;

    // keyboard
    io.ClearInputCharacters();
    io.AddInputCharactersUTF8(keyboardState.text.data());

    static_assert(sizeof(io.KeysDown) >= sizeof(keyboardState.keysDown));
    std::memcpy(io.KeysDown, keyboardState.keysDown.data(), sizeof(keyboardState.keysDown));
    io.KeyShift = keyboardState.shiftMod;
    io.KeyCtrl  = keyboardState.ctrlMod;
    io.KeyAlt   = keyboardState.altMod;
    io.KeySuper = keyboardState.superMod;

    /* remove */
    SDL_ShowCursor(io.MouseDrawCursor ? 0 : 1);
    /* end remove */

    ::ImGui::NewFrame();

    return frame++;
}

void ImGui::EndFrame()
{
    ::ImGui::Render();
    DrawLists(::ImGui::GetDrawData());
}

void ImGui::DrawLists(ImDrawData* draw_data)
{
    if (draw_data == nullptr)
        return;

    // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
    ImGuiIO& io        = ::ImGui::GetIO();
    int      fb_width  = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
    int      fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
    if (fb_width == 0 || fb_height == 0)
        return;

    draw_data->ScaleClipRects(io.DisplayFramebufferScale);

    auto& renderer = Get<Renderer>();
    auto& cb       = renderer.GetCommandBuffer();

    // Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, polygon fill
    cb.BindBlendState(blendState);
    cb.BindDepthState(depthState);
    cb.BindRasterizerState(rasterizerState);

    // Setup viewport, orthographic projection matrix
    cb.SetViewport(0, 0, fb_width, fb_height);

    // Setup shader and vertex input
    cb.BindShaderState(shader, vtxLayout);

    const float ortho_projection[4][4] = {
        { +2.0f / io.DisplaySize.x, /*                */ 0.0f, +0.0f, +0.0f },
        { +0.0f /*              */, +2.0f / -io.DisplaySize.y, +0.0f, +0.0f },
        { +0.0f /*              */, /*                */ 0.0f, -1.0f, +0.0f },
        { -1.0f /*              */, /*                */ 1.0f, +0.0f, +1.0f },
    };

    auto ubo = renderer.CreateTempBuffer({ BufferType::Constant, sizeof(ortho_projection) }, ortho_projection);
    cb.BindConstantBuffer(ubo, 0);

    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list          = draw_data->CmdLists[n];
        int               idx_buffer_offset = 0;

        auto vertex = renderer.CreateTempBuffer(
            { BufferType::Vertex, static_cast<uint32_t>(cmd_list->VtxBuffer.Size * sizeof(ImDrawVert)) },
            cmd_list->VtxBuffer.Data);
        cb.BindVertexBuffer(vertex, 0);

        auto index = renderer.CreateTempBuffer(
            { BufferType::Index, static_cast<uint32_t>(cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx)) },
            cmd_list->IdxBuffer.Data);
        cb.BindIndexBuffer(index);

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback)
            {
                pcmd->UserCallback(cmd_list, pcmd);
            }
            else
            {
                cb.BindTexture(*static_cast<TexturePtr*>(pcmd->TextureId), 0);
                cb.SetScissor(pcmd->ClipRect.x, fb_height - pcmd->ClipRect.w, pcmd->ClipRect.z - pcmd->ClipRect.x, pcmd->ClipRect.w - pcmd->ClipRect.y);
                cb.DrawIndexed(pcmd->ElemCount, 1, idx_buffer_offset, 0, 0);
            }
            idx_buffer_offset += pcmd->ElemCount;
        }
    }
}

} // namespace p7::gfx
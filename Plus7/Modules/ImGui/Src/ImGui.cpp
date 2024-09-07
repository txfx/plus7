#include "ImGui.hpp"

#include "SDL_mouse.h"
#include "SDL_scancode.h"

#include <KeyboardState.hpp>
#include <MouseState.hpp>
#include <Tasks/Pipeline.hpp>
#include <Utils/Assert.hpp>

#include <cstring>
#include <imgui.h>
#include <ranges>

#if IMGUI_MODULE_NEED_SDL
#include <SDL.h>
#endif

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

constexpr VertexLayoutProperties vtxLayoutProperties { { VertexAttribute::Type::XYZW32F, 0 },
                                                       { VertexAttribute::Type::R8G8B8A8_UNORM, 0 } };
#if IMGUI_MODULE_NEED_SDL
ImGuiKey ConvertSDLKeyToImGuiKey(SDL_Scancode sdlScancode)
{
    switch (sdlScancode)
    {
    case SDL_SCANCODE_TAB:
        return ImGuiKey_Tab;
    case SDL_SCANCODE_LEFT:
        return ImGuiKey_LeftArrow;
    case SDL_SCANCODE_RIGHT:
        return ImGuiKey_RightArrow;
    case SDL_SCANCODE_UP:
        return ImGuiKey_UpArrow;
    case SDL_SCANCODE_DOWN:
        return ImGuiKey_DownArrow;
    case SDL_SCANCODE_PAGEUP:
        return ImGuiKey_PageUp;
    case SDL_SCANCODE_PAGEDOWN:
        return ImGuiKey_PageDown;
    case SDL_SCANCODE_HOME:
        return ImGuiKey_Home;
    case SDL_SCANCODE_END:
        return ImGuiKey_End;
    case SDL_SCANCODE_INSERT:
        return ImGuiKey_Insert;
    case SDL_SCANCODE_DELETE:
        return ImGuiKey_Delete;
    case SDL_SCANCODE_BACKSPACE:
        return ImGuiKey_Backspace;
    case SDL_SCANCODE_SPACE:
        return ImGuiKey_Space;
    case SDL_SCANCODE_RETURN:
        return ImGuiKey_Enter;
    case SDL_SCANCODE_ESCAPE:
        return ImGuiKey_Escape;
    case SDL_SCANCODE_APOSTROPHE:
        return ImGuiKey_Apostrophe;
    case SDL_SCANCODE_COMMA:
        return ImGuiKey_Comma;
    case SDL_SCANCODE_MINUS:
        return ImGuiKey_Minus;
    case SDL_SCANCODE_PERIOD:
        return ImGuiKey_Period;
    case SDL_SCANCODE_SLASH:
        return ImGuiKey_Slash;
    case SDL_SCANCODE_SEMICOLON:
        return ImGuiKey_Semicolon;
    case SDL_SCANCODE_EQUALS:
        return ImGuiKey_Equal;
    case SDL_SCANCODE_LEFTBRACKET:
        return ImGuiKey_LeftBracket;
    case SDL_SCANCODE_BACKSLASH:
        return ImGuiKey_Backslash;
    case SDL_SCANCODE_RIGHTBRACKET:
        return ImGuiKey_RightBracket;
    case SDL_SCANCODE_GRAVE:
        return ImGuiKey_GraveAccent;
    case SDL_SCANCODE_CAPSLOCK:
        return ImGuiKey_CapsLock;
    case SDL_SCANCODE_SCROLLLOCK:
        return ImGuiKey_ScrollLock;
    case SDL_SCANCODE_NUMLOCKCLEAR:
        return ImGuiKey_NumLock;
    case SDL_SCANCODE_PRINTSCREEN:
        return ImGuiKey_PrintScreen;
    case SDL_SCANCODE_PAUSE:
        return ImGuiKey_Pause;
    case SDL_SCANCODE_KP_0:
        return ImGuiKey_Keypad0;
    case SDL_SCANCODE_KP_1:
        return ImGuiKey_Keypad1;
    case SDL_SCANCODE_KP_2:
        return ImGuiKey_Keypad2;
    case SDL_SCANCODE_KP_3:
        return ImGuiKey_Keypad3;
    case SDL_SCANCODE_KP_4:
        return ImGuiKey_Keypad4;
    case SDL_SCANCODE_KP_5:
        return ImGuiKey_Keypad5;
    case SDL_SCANCODE_KP_6:
        return ImGuiKey_Keypad6;
    case SDL_SCANCODE_KP_7:
        return ImGuiKey_Keypad7;
    case SDL_SCANCODE_KP_8:
        return ImGuiKey_Keypad8;
    case SDL_SCANCODE_KP_9:
        return ImGuiKey_Keypad9;
    case SDL_SCANCODE_KP_PERIOD:
        return ImGuiKey_KeypadDecimal;
    case SDL_SCANCODE_KP_DIVIDE:
        return ImGuiKey_KeypadDivide;
    case SDL_SCANCODE_KP_MULTIPLY:
        return ImGuiKey_KeypadMultiply;
    case SDL_SCANCODE_KP_MINUS:
        return ImGuiKey_KeypadSubtract;
    case SDL_SCANCODE_KP_PLUS:
        return ImGuiKey_KeypadAdd;
    case SDL_SCANCODE_KP_ENTER:
        return ImGuiKey_KeypadEnter;
    case SDL_SCANCODE_KP_EQUALS:
        return ImGuiKey_KeypadEqual;
    case SDL_SCANCODE_LCTRL:
        return ImGuiKey_LeftCtrl;
    case SDL_SCANCODE_LSHIFT:
        return ImGuiKey_LeftShift;
    case SDL_SCANCODE_LALT:
        return ImGuiKey_LeftAlt;
    case SDL_SCANCODE_LGUI:
        return ImGuiKey_LeftSuper;
    case SDL_SCANCODE_RCTRL:
        return ImGuiKey_RightCtrl;
    case SDL_SCANCODE_RSHIFT:
        return ImGuiKey_RightShift;
    case SDL_SCANCODE_RALT:
        return ImGuiKey_RightAlt;
    case SDL_SCANCODE_RGUI:
        return ImGuiKey_RightSuper;
    case SDL_SCANCODE_APPLICATION:
        return ImGuiKey_Menu;
    case SDL_SCANCODE_0:
        return ImGuiKey_0;
    case SDL_SCANCODE_1:
        return ImGuiKey_1;
    case SDL_SCANCODE_2:
        return ImGuiKey_2;
    case SDL_SCANCODE_3:
        return ImGuiKey_3;
    case SDL_SCANCODE_4:
        return ImGuiKey_4;
    case SDL_SCANCODE_5:
        return ImGuiKey_5;
    case SDL_SCANCODE_6:
        return ImGuiKey_6;
    case SDL_SCANCODE_7:
        return ImGuiKey_7;
    case SDL_SCANCODE_8:
        return ImGuiKey_8;
    case SDL_SCANCODE_9:
        return ImGuiKey_9;
    case SDL_SCANCODE_A:
        return ImGuiKey_A;
    case SDL_SCANCODE_B:
        return ImGuiKey_B;
    case SDL_SCANCODE_C:
        return ImGuiKey_C;
    case SDL_SCANCODE_D:
        return ImGuiKey_D;
    case SDL_SCANCODE_E:
        return ImGuiKey_E;
    case SDL_SCANCODE_F:
        return ImGuiKey_F;
    case SDL_SCANCODE_G:
        return ImGuiKey_G;
    case SDL_SCANCODE_H:
        return ImGuiKey_H;
    case SDL_SCANCODE_I:
        return ImGuiKey_I;
    case SDL_SCANCODE_J:
        return ImGuiKey_J;
    case SDL_SCANCODE_K:
        return ImGuiKey_K;
    case SDL_SCANCODE_L:
        return ImGuiKey_L;
    case SDL_SCANCODE_M:
        return ImGuiKey_M;
    case SDL_SCANCODE_N:
        return ImGuiKey_N;
    case SDL_SCANCODE_O:
        return ImGuiKey_O;
    case SDL_SCANCODE_P:
        return ImGuiKey_P;
    case SDL_SCANCODE_Q:
        return ImGuiKey_Q;
    case SDL_SCANCODE_R:
        return ImGuiKey_R;
    case SDL_SCANCODE_S:
        return ImGuiKey_S;
    case SDL_SCANCODE_T:
        return ImGuiKey_T;
    case SDL_SCANCODE_U:
        return ImGuiKey_U;
    case SDL_SCANCODE_V:
        return ImGuiKey_V;
    case SDL_SCANCODE_W:
        return ImGuiKey_W;
    case SDL_SCANCODE_X:
        return ImGuiKey_X;
    case SDL_SCANCODE_Y:
        return ImGuiKey_Y;
    case SDL_SCANCODE_Z:
        return ImGuiKey_Z;
    case SDL_SCANCODE_F1:
        return ImGuiKey_F1;
    case SDL_SCANCODE_F2:
        return ImGuiKey_F2;
    case SDL_SCANCODE_F3:
        return ImGuiKey_F3;
    case SDL_SCANCODE_F4:
        return ImGuiKey_F4;
    case SDL_SCANCODE_F5:
        return ImGuiKey_F5;
    case SDL_SCANCODE_F6:
        return ImGuiKey_F6;
    case SDL_SCANCODE_F7:
        return ImGuiKey_F7;
    case SDL_SCANCODE_F8:
        return ImGuiKey_F8;
    case SDL_SCANCODE_F9:
        return ImGuiKey_F9;
    case SDL_SCANCODE_F10:
        return ImGuiKey_F10;
    case SDL_SCANCODE_F11:
        return ImGuiKey_F11;
    case SDL_SCANCODE_F12:
        return ImGuiKey_F12;
    case SDL_SCANCODE_F13:
        return ImGuiKey_F13;
    case SDL_SCANCODE_F14:
        return ImGuiKey_F14;
    case SDL_SCANCODE_F15:
        return ImGuiKey_F15;
    case SDL_SCANCODE_F16:
        return ImGuiKey_F16;
    case SDL_SCANCODE_F17:
        return ImGuiKey_F17;
    case SDL_SCANCODE_F18:
        return ImGuiKey_F18;
    case SDL_SCANCODE_F19:
        return ImGuiKey_F19;
    case SDL_SCANCODE_F20:
        return ImGuiKey_F20;
    case SDL_SCANCODE_F21:
        return ImGuiKey_F21;
    case SDL_SCANCODE_F22:
        return ImGuiKey_F22;
    case SDL_SCANCODE_F23:
        return ImGuiKey_F23;
    case SDL_SCANCODE_F24:
        return ImGuiKey_F24;
    case SDL_SCANCODE_AC_BACK:
        return ImGuiKey_AppBack;
    case SDL_SCANCODE_AC_FORWARD:
        return ImGuiKey_AppForward;
    default:
        return ImGuiKey_None;
    }
}
#endif

} // namespace

using namespace p7::tasks;

ImGui::ImGui(
  tasks::Pipeline&               _pipeline,
  TypedID<inputs::MouseState>    _mouseTask,
  TypedID<inputs::KeyboardState> _keyboardTask,
  Renderer&                      _renderer)
    : beginFrameTask(_pipeline.AddTask(
        "ImGui begin frame"_name,
        consuming(_mouseTask, _keyboardTask.prev(), _keyboardTask),
        [&](const auto& mouseState, const auto& prevKeyboardState, const auto& keyboardState) {
            return this->BeginFrame(mouseState, prevKeyboardState, keyboardState, _renderer);
        }))
    , endFrameTask(_pipeline.AddTask(
        "ImGui end frame"_name,
        consuming(beginFrameTask),
        before(_renderer.GetEndFrameTask()),
        [&](uint64_t) { this->EndFrame(_renderer); }))
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
    font = _renderer.CreateTexture({ static_cast<uint32_t>(width), static_cast<uint32_t>(height) }, pixels);

    io.Fonts->TexID = &font;

    static_assert(sizeof(ImDrawVert) == sizeof(ImVec2) * 2 + sizeof(ImU32));
}

ImGui::~ImGui() { ::ImGui::DestroyContext(context); }

uint64_t ImGui::BeginFrame(
  const inputs::MouseState&    _mouseState,
  const inputs::KeyboardState& _prevKeyboardState,
  const inputs::KeyboardState& _keyboardState,
  const Renderer&              _renderer)
{
    ImGuiIO& io    = ::ImGui::GetIO();
    io.DisplaySize = ImVec2(_renderer.GetWidth(), _renderer.GetHeight());

    // mouse
    io.MousePos = ImVec2(_mouseState.pos.x, _mouseState.pos.y);

    io.MouseDown[0] = _mouseState.buttons[0];
    io.MouseDown[1] = _mouseState.buttons[1];
    io.MouseDown[2] = _mouseState.buttons[2];

    io.MouseWheel  = _mouseState.wheel.vertical;
    io.MouseWheelH = _mouseState.wheel.horizontal;

    // keyboard
    io.AddInputCharactersUTF8(_keyboardState.text.data());

#if IMGUI_MODULE_NEED_SDL
    for (const auto [id, prevIsDown, isDown] :
         std::views::zip(std::views::iota(0), _prevKeyboardState.keysDown, _keyboardState.keysDown))
    {
        if (prevIsDown != isDown)
            io.AddKeyEvent(ConvertSDLKeyToImGuiKey(static_cast<SDL_Scancode>(id)), isDown);
    }
#endif
    io.KeyShift = _keyboardState.shiftMod;
    io.KeyCtrl  = _keyboardState.ctrlMod;
    io.KeyAlt   = _keyboardState.altMod;
    io.KeySuper = _keyboardState.superMod;

#if IMGUI_MODULE_NEED_SDL
    SDL_ShowCursor(io.MouseDrawCursor ? 0 : 1);

    uint32_t now = SDL_GetTicks();
    io.DeltaTime = (now - lastTick) / 1000.0f;
    lastTick     = now;
#endif

    ::ImGui::NewFrame();

    return frame++;
}

void ImGui::EndFrame(Renderer& _renderer)
{
    ::ImGui::Render();
    DrawLists(::ImGui::GetDrawData(), _renderer);
}

void ImGui::DrawLists(ImDrawData* draw_data, Renderer& _renderer)
{
    if (draw_data == nullptr)
        return;

    // Avoid rendering when minimized, scale coordinates for retina displays
    // (screen coordinates != framebuffer coordinates)
    ImGuiIO& io        = ::ImGui::GetIO();
    int      fb_width  = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
    int      fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
    if (fb_width == 0 || fb_height == 0)
        return;

    draw_data->ScaleClipRects(io.DisplayFramebufferScale);

    auto& cb = _renderer.GetCommandBuffer();

    // Setup render state: alpha-blending enabled, no face culling, no depth
    // testing, scissor enabled, polygon fill
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

    auto ubo = _renderer.CreateTempBuffer({ BufferType::Constant, sizeof(ortho_projection) }, ortho_projection);
    cb.BindConstantBuffer(ubo, 0);

    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const auto* cmd_list          = draw_data->CmdLists[n];
        int         idx_buffer_offset = 0;

        auto vertex = _renderer.CreateTempBuffer(
          { BufferType::Vertex, static_cast<uint32_t>(cmd_list->VtxBuffer.Size * sizeof(ImDrawVert)) },
          cmd_list->VtxBuffer.Data);
        cb.BindVertexBuffer(vertex, 0);

        auto index = _renderer.CreateTempBuffer(
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
                cb.SetScissor(
                  pcmd->ClipRect.x,
                  fb_height - pcmd->ClipRect.w,
                  pcmd->ClipRect.z - pcmd->ClipRect.x,
                  pcmd->ClipRect.w - pcmd->ClipRect.y);
                cb.DrawIndexed(pcmd->ElemCount, 1, idx_buffer_offset, 0, 0);
            }
            idx_buffer_offset += pcmd->ElemCount;
        }
    }
}

} // namespace p7::gfx
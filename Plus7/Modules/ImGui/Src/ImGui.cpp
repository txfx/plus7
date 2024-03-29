#include "ImGui.hpp"

#include <KeyboardState.hpp>
#include <MouseState.hpp>
#include <Tasks/Pipeline.hpp>
#include <Utils/Assert.hpp>

#include <cstring>
#include <imgui.h>

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

} // namespace

using namespace p7::tasks;

ImGui::ImGui(
  tasks::Pipeline&               _pipeline,
  TypedID<inputs::MouseState>    _mouseTask,
  TypedID<inputs::KeyboardState> _keyboardTask,
  Renderer&                      _renderer)
    : beginFrameTask(_pipeline.AddTask(
      "ImGui begin frame"_name,
      consuming(_mouseTask, _keyboardTask),
      [&](const auto& mouseState, const auto& keyboardState) {
          return this->BeginFrame(mouseState, keyboardState, _renderer);
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

#if IMGUI_MODULE_NEED_SDL
    // Keyboard mapping. ImGui will use those indices to peek into the
    // io.KeysDown[] array.
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
#endif // HACK
}

ImGui::~ImGui() { ::ImGui::DestroyContext(context); }

uint64_t ImGui::BeginFrame(
  const inputs::MouseState&    _mouseState,
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
    io.ClearInputCharacters();
    io.AddInputCharactersUTF8(_keyboardState.text.data());

    static_assert(sizeof(io.KeysDown) >= sizeof(_keyboardState.keysDown));
    std::memcpy(io.KeysDown, _keyboardState.keysDown.data(), sizeof(_keyboardState.keysDown));
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
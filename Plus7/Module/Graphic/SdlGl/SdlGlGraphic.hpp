#pragma once

#include <Module.hpp>

struct SDL_Window;
using SDL_GLContext = void*;

namespace p7 {
namespace module {
struct SdlGlGraphic : public Module
{
public:
    explicit SdlGlGraphic(App& _app);
    SdlGlGraphic(const char* _name, int _w, int _h, App& _app);
    ~SdlGlGraphic() override;

    void Update() override;

private:
    SdlGlGraphic(const char* _name, int _w, int _h, bool _visible, App& _app);

private:
    int           width;
    int           height;
    bool          initialized;
    SDL_Window*   window;
    SDL_GLContext glcontext;
};
} // namespace module
} // namespace p7
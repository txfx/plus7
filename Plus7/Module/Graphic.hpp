#pragma once

#include "Base.hpp"

struct SDL_Window;
using SDL_GLContext = void*;

namespace p7 {
namespace module {
struct Graphic : public Base
{
public:
    Graphic(App& _app);
    Graphic(const char* _name, int _w, int _h, App& _app);
    ~Graphic() override;

    void Update() override;

private:
    Graphic(const char* _name, int _w, int _h, bool _visible, App& _app);

private:
    int           width;
    int           height;
    bool          initialized;
    SDL_Window*   window;
    SDL_GLContext glcontext;
};
} // namespace module
} // namespace p7
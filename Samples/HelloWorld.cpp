#include <iostream>

#include <App.hpp>
#include <Module/Graphic/Renderer.hpp>

int main()
{
    using p7::App;
    using p7::gfx::Renderer;

    App helloWorldApp;
    helloWorldApp.LoadModule<Renderer>("Hello World", 640, 480);

    helloWorldApp.Run();
    std::cout << "Hello World" << std::endl;
}
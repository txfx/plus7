#include <iostream>

#include <App.hpp>
#include <Module/Graphic/SdlGlGraphic.hpp>

int main()
{
    using p7::App;
    using p7::module::SdlGlGraphic;

    App helloWorldApp;
    helloWorldApp.LoadModule<SdlGlGraphic>("Hello World", 640, 480);

    helloWorldApp.Run();
    std::cout << "Hello World" << std::endl;
}
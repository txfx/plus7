#include <iostream>

#include <App.hpp>
#include <Module/Graphic/Graphic.hpp>

int main()
{
    using p7::App;
    using p7::module::Graphic;

    App helloWorldApp;
    helloWorldApp.LoadModule<Graphic>("Hello World", 640, 480);

    helloWorldApp.Run();
    std::cout << "Hello World" << std::endl;
}
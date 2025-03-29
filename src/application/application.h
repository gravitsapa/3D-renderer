#pragma once

#include <string>
#include <renderer.h>
#include <SFML/Graphics.hpp>

namespace project {
namespace application {

class Application {
public:
    Application(kernel::Height screen_height, kernel::Width screen_width);

    void Run();

private:
    void LoadSceneWithCube();
    void LoadSceneWithCoffee();
    void ShowScreen();

    sf::RenderWindow window_;
    kernel::World world_;
    kernel::Screen screen_;
    kernel::Renderer renderer_;
};

}  // namespace application
}  // namespace project

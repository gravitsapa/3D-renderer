#include <application.h>
#include <iostream>
#include <world.h>
#include <renderer.h>
#include <SFML/Graphics.hpp>

namespace project {
namespace application {
Application::Application(kernel::Height screen_height, kernel::Width screen_width)
    : screen_(screen_height, screen_width),
      window_(sf::VideoMode({screen_width, screen_height}), "CMake SFML Project") {
    world_.AddObject(kernel::Object::CreateRectangularСuboid(130, 100, 150),
                     geometry::Pose{geometry::Rotation::ByAngles(0, 0.75, 0.75),
                                    geometry::Position{geometry::Vector3d{0, 0, -200}}});

    world_.AddCamera(kernel::Camera{50, 500, 200, 200, 300, 300});
}

void Application::Run() {
    while (window_.isOpen()) {
        kernel::Renderer renderer;
        screen_ = renderer.Project(world_, world_.GetCameras()[0], std::move(screen_));

        while (const std::optional event = window_.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window_.close();
            }
        }
        window_.clear(sf::Color::Black);
        sf::Texture texture;
        texture.loadFromImage(screen_.ConvertToImage());
        sf::Sprite sprite(texture);
        window_.draw(sprite);
        window_.display();
    }
}

}  // namespace application
}  // namespace project

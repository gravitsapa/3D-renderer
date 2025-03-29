#include <application.h>
#include <iostream>
#include <world.h>
#include <renderer.h>
#include <file_reader.h>
#include <SFML/Graphics.hpp>

namespace project {
namespace application {
Application::Application(kernel::Height screen_height, kernel::Width screen_width)
    : screen_(screen_height, screen_width),
      window_(sf::VideoMode({screen_width, screen_height}), "3D-renderer") {
    LoadSampleScene();
}

void Application::LoadSampleScene() {
    std::string object_folder_path = "../objects/";
    std::string plant_mesh_path = object_folder_path + "plant/object.obj";
    std::string plant_texture_path = object_folder_path + "plant/texture.png";
    auto plant = kernel::Object{kernel::ReadMeshFromFile(plant_mesh_path),
                       kernel::ReadTextureFromFile(plant_texture_path)};

    kernel::PrintDebugInfo(plant, "PLANT");

    world_.AddObject(
        plant,
        geometry::Pose{geometry::Rotation(), geometry::Position{geometry::Vector3d{0, 0, -40}}});

    world_.AddCamera(kernel::Camera{30, 200, 100, 100, 75, 75});
}

void Application::ShowScreen() {
    sf::Texture texture;
    texture.loadFromImage(screen_.ConvertToImage());
    sf::Sprite sprite(texture);
    window_.draw(sprite);
    window_.display();
}

void Application::Run() {
    screen_ = renderer_.Project(world_, world_.GetCameras()[0], std::move(screen_));

    while (window_.isOpen()) {
        while (const std::optional event = window_.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window_.close();
            }
        }

        ShowScreen();
    }
}

}  // namespace application
}  // namespace project

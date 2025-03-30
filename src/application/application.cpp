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
    LoadSceneWithCoffee();
}

void Application::LoadSceneWithCoffee() {
    std::string object_folder_path = "../objects/";
    std::string plant_mesh_path = object_folder_path + "coffee/object.obj";
    std::string plant_texture_path = object_folder_path + "coffee/texture.png";
    auto plant = kernel::Object{kernel::ReadMeshFromFile(plant_mesh_path),
                       kernel::ReadTextureFromFile(plant_texture_path)};

    kernel::PrintDebugInfo(plant, "COFFEE");

    world_.AddObject(
        plant,
        geometry::Pose{geometry::Rotation::ByAngles(3.14, 0.5, 0), geometry::Position{geometry::Vector3d{0, 0.4, -0.8}}});

    double f = 0.5;
    world_.AddCamera(kernel::Camera{0.5, 2, f * 1, f * 1, f * 0.75, f * 0.75});

    kernel::Lights lights;
    lights.AddAmbientLight(kernel::AmbientLight(kernel::Color::White() * 0.3));
    lights.AddDirectionalLight(kernel::DirectionalLight(geometry::Vector3d(-1, 1, -0.25),kernel::Color::White()));
    world_.AddLights(lights);
}

void Application::LoadSceneWithCube() {
    auto rec = kernel::Object{.mesh = kernel::Mesh3d::RectangularСuboid(100, 150, 200), .texture = kernel::Texture()};
    kernel::PrintDebugInfo(rec, "RECTANGLE");
    world_.AddObject(
        rec,
        geometry::Pose{geometry::Rotation::ByAngles(0, 0.75, 0.75), geometry::Position{geometry::Vector3d{0, 0, -200}}});

    world_.AddCamera(kernel::Camera{50, 500, 200, 200, 150, 150});
    kernel::Lights lights;
    lights.AddAmbientLight(kernel::AmbientLight(kernel::Color::White() * 0.3));
    lights.AddDirectionalLight(kernel::DirectionalLight(geometry::Vector3d(-1, 1, -0.25),kernel::Color::White()));
    world_.AddLights(lights);
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

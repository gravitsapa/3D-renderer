#include <application.h>
#include <iostream>
#include <renderer.h>
#include <file_reader.h>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <timer.h>

namespace project {
namespace application {
Application::Application(kernel::Height screen_height, kernel::Width screen_width,
                         std::string scene_name)
    : screen_(screen_height, screen_width),
      window_(sf::VideoMode({screen_width, screen_height}), "3D-renderer") {

    if (scene_name == "coffee") {
        LoadSceneWithCoffee();
    } else if (scene_name == "house") {
        LoadSceneWithHouse();
    } else if (scene_name == "chess") {
        LoadSceneWithChess();
    } else {
        LoadEmptyScene();
    }
}

void Application::LoadEmptyScene() {
    world_.AddCamera(kernel::Camera(1, 2, 1, 1, 1, 1));
    frame_processor_ = [](kernel::World& world) -> const kernel::PosedCamera& {
        return world.GetCamera(0);
    };
}

void Application::LoadSceneWithHouse() {
    std::string object_folder_path = "../objects/";
    std::string house_mesh_path = object_folder_path + "house/object.obj";
    std::string house_texture_path = object_folder_path + "house/texture.png";
    auto house = kernel::ReadObjectFromFiles(house_mesh_path, house_texture_path);

    kernel::PrintDebugInfo(house, "HOUSE");

    world_.AddObject(house, geometry::Pose{geometry::Rotation::ByAngles(3.14, 0.9, 0.5),
                                           geometry::Position{geometry::Point3d{0, 5, -50}}});

    world_.AddCamera(kernel::Camera{3, 100, 2.5, 2.5, 3.75 / 2, 3.75 / 2});

    kernel::Lights lights;
    lights.AddAmbientLight(kernel::AmbientLight(kernel::Color::White() * 0.3));
    lights.AddDirectionalLight(
        kernel::DirectionalLight(geometry::Vector3d(-1, 1, -0.25), kernel::Color::White()));
    world_.AddLights(lights);

    frame_processor_ = detail::RotateProcessor;
}

void Application::LoadSceneWithCoffee() {
    std::string object_folder_path = "../objects/";
    std::string coffee_mesh_path = object_folder_path + "coffee/object.obj";
    std::string coffee_texture_path = object_folder_path + "coffee/texture.png";
    auto coffee = kernel::ReadObjectFromFiles(coffee_mesh_path, coffee_texture_path);

    kernel::PrintDebugInfo(coffee, "COFFEE");

    world_.AddObject(coffee, geometry::Pose{geometry::Rotation::ByAngles(3.14, 0.5, 0),
                                            geometry::Position{geometry::Vector3d{0, 0.4, -0.8}}});

    world_.AddCamera(kernel::Camera{0.5, 2, 0.5, 0.5, 0.375, 0.375});

    kernel::Lights lights;
    lights.AddAmbientLight(kernel::AmbientLight(kernel::Color::White() * 0.3));
    lights.AddDirectionalLight(
        kernel::DirectionalLight(geometry::Vector3d(-1, 1, -0.25), kernel::Color::White()));
    world_.AddLights(lights);

    frame_processor_ = detail::RotateProcessor;
}

void Application::LoadSceneWithChess() {
    std::string object_folder_path = "../objects/";
    std::string plant_mesh_path = object_folder_path + "chess/object.obj";
    std::string plant_texture_path = object_folder_path + "chess/texture.png";
    auto chess = kernel::Object{kernel::ReadMeshFromFile(plant_mesh_path),
                                kernel::ReadTextureFromFile(plant_texture_path)};

    kernel::PrintDebugInfo(chess, "CHESS");

    world_.AddObject(chess, geometry::Pose{geometry::Rotation::ByAngles(0.2, 0, 0.5),
                                           geometry::Position{geometry::Vector3d{-0.5, -0.5, -2}}});

    world_.AddCamera(kernel::Camera{0.5, 5, 0.5, 0.5, 0.375, 0.375});

    kernel::Lights lights;
    lights.AddAmbientLight(kernel::AmbientLight(kernel::Color::White() * 0.3));
    lights.AddDirectionalLight(
        kernel::DirectionalLight(geometry::Vector3d(-1, 1, -0.25), kernel::Color::White()));
    world_.AddLights(lights);

    frame_processor_ = detail::RotateProcessor;
}

void Application::ShowScreen() {
    sf::Texture texture;
    texture.loadFromImage(screen_.ConvertToImage());
    sf::Sprite sprite(texture);
    window_.draw(sprite);
    window_.display();
}

void Application::Run() {
    Timer timer(5 * CLOCKS_PER_SEC);
    int frames_done = 0;
    while (window_.isOpen()) {
        while (const std::optional event = window_.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window_.close();
            }
        }

        auto camera = frame_processor_(world_);
        screen_ = renderer_.Project(world_, camera, std::move(screen_));
        ShowScreen();

        frames_done++;
        if (timer.Tick()) {
            double mean_fps = static_cast<double>(frames_done) / timer.TimeInSeconds();
            std::cout << "Mean fps: " << mean_fps << std::endl;
        }
    }
}

namespace detail {
const kernel::PosedCamera& RotateProcessor(kernel::World& world) {
    static Timer timer = Timer();
    double alpha = timer.TimeInSeconds() / 3;

    auto new_pose = geometry::Pose(world.GetObject(0));
    new_pose.rot_matrix = geometry::Rotation::ByAngles(alpha, alpha, alpha).rot_matrix;

    world.MoveObject(0, new_pose);
    return world.GetCamera(0);
}
}  // namespace detail

}  // namespace application
}  // namespace project

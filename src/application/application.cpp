#include <application.h>
#include <iostream>
#include <renderer.h>
#include <file_reader.h>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <timer.h>
#include <fps_counter.h>
#include <mouse_tracker.h>

namespace project {
namespace application {
Application::Application(kernel::Height screen_height, kernel::Width screen_width,
                         std::string scene_name)
    : screen_(screen_height, screen_width),
      window_(sf::VideoMode({screen_width, screen_height}), "3D-renderer"),
      renderer_(screen_height, screen_width) {

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

    world_.AddObject(house, geometry::Pose{geometry::Rotation(),
                                           geometry::Position{geometry::Point3d{0, 5, -50}}});

    detail::CameraPlaneSize camera_plane_size = ExpandSizeAccordingToResolution(
        detail::CameraPlaneSize{2.5, 2}, screen_.GetHeight(), screen_.GetWidth());

    world_.AddCamera(kernel::Camera{3, 100, camera_plane_size.w, camera_plane_size.w,
                                    camera_plane_size.h, camera_plane_size.h});

    kernel::Lights lights;
    lights.AddAmbientLight(kernel::AmbientLight(kernel::Color::White() * 0.3));
    lights.AddDirectionalLight(
        kernel::DirectionalLight(geometry::Vector3d(-1, -1, -0.25), kernel::Color::White()));
    world_.AddLights(lights);

    frame_processor_ = detail::InteractiveProcessor();
}

void Application::LoadSceneWithCoffee() {
    std::string object_folder_path = "../objects/";
    std::string coffee_mesh_path = object_folder_path + "coffee/object.obj";
    std::string coffee_texture_path = object_folder_path + "coffee/texture.png";
    auto coffee = kernel::ReadObjectFromFiles(coffee_mesh_path, coffee_texture_path);

    kernel::PrintDebugInfo(coffee, "COFFEE");

    world_.AddObject(coffee,
                     geometry::Pose{geometry::Rotation(),
                                    geometry::Position{geometry::Vector3d{0.2, -0.4, -0.8}}});

    detail::CameraPlaneSize camera_plane_size = ExpandSizeAccordingToResolution(
        detail::CameraPlaneSize{0.5, 0.5}, screen_.GetHeight(), screen_.GetWidth());
    world_.AddCamera(kernel::Camera{0.5, 2, camera_plane_size.w, camera_plane_size.w,
                                    camera_plane_size.h, camera_plane_size.h});

    kernel::Lights lights;
    lights.AddAmbientLight(kernel::AmbientLight(kernel::Color::White() * 0.3));
    lights.AddDirectionalLight(
        kernel::DirectionalLight(geometry::Vector3d(-1, -1, -0.25), kernel::Color::White()));
    world_.AddLights(lights);

    frame_processor_ = detail::InteractiveProcessor();
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

    detail::CameraPlaneSize camera_plane_size = ExpandSizeAccordingToResolution(
        detail::CameraPlaneSize{0.5, 0.5}, screen_.GetHeight(), screen_.GetWidth());
    world_.AddCamera(kernel::Camera{0.5, 5, camera_plane_size.w, camera_plane_size.w,
                                    camera_plane_size.h, camera_plane_size.h});

    kernel::Lights lights;
    lights.AddAmbientLight(kernel::AmbientLight(kernel::Color::White() * 0.3));
    lights.AddDirectionalLight(
        kernel::DirectionalLight(geometry::Vector3d(-1, -1, -0.25), kernel::Color::White()));
    world_.AddLights(lights);

    frame_processor_ = detail::InteractiveProcessor();
}

void Application::ShowScreen() {
    sf::Texture texture;
    texture.loadFromImage(screen_.ConvertToImage());
    sf::Sprite sprite(texture);
    window_.draw(sprite);
    window_.display();
}

void Application::Run() {
    FpsCounter fps_counter(5);

    while (window_.isOpen()) {
        while (const std::optional event = window_.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window_.close();
            }
        }

        auto camera = frame_processor_(world_);
        screen_ = renderer_.Project(world_, camera, std::move(screen_));
        ShowScreen();

        fps_counter.Frame();
    }
}

namespace detail {

CameraPlaneSize ExpandSizeAccordingToResolution(CameraPlaneSize size, kernel::Height height,
                                                kernel::Width width) {
    if (height * size.w > size.h * width) {
        return CameraPlaneSize{.h = size.w * height / width, .w = size.w};
    }
    return CameraPlaneSize{.h = size.h, .w = size.h * width / height};
}

// const kernel::PosedCamera& RotateProcessor(kernel::World& world) {
//     static Timer timer = Timer();
//     double alpha = timer.TimeInSeconds() / 3;

//     auto new_pose = geometry::Pose(world.GetObject(0));
//     new_pose.pos_vector.y() = alpha / 10 - 1;
//     new_pose.rot_matrix = geometry::Rotation::ByAngles(0, alpha, 0).rot_matrix;

//     world.MoveObject(0, new_pose);
//     return world.GetCamera(0);
// }

geometry::Coordinate InteractiveProcessor::ConvertToGeometryCoordinate(int coord) {
    return static_cast<geometry::Coordinate>(coord);
}

geometry::Pose InteractiveProcessor::MovePoseByMouseDisplacement(geometry::Pose pose,
                                                                 sf::Vector2i displacement) {
    static constexpr int pixels_per_radian = 300;
    pose.rot_matrix =
        geometry::Rotation::ByAngles(-ConvertToGeometryCoordinate(displacement.x) / pixels_per_radian, 0,
                                     ConvertToGeometryCoordinate(displacement.y) / pixels_per_radian)
            .rot_matrix *
        pose.rot_matrix;

    return pose;
}

const kernel::PosedCamera& InteractiveProcessor::operator()(kernel::World& world) {
    mouse_tracker_.Frame();

    auto showing_pose_now = geometry::Pose(world.GetObject(0));

    geometry::Pose new_pose;
    if (mouse_tracker_.IsLeftButtonHolded()) {
        new_pose = MovePoseByMouseDisplacement(real_object_pose_,
                                               mouse_tracker_.DisplacementWhileLeftHolded());
    } else {
        if (mouse_tracker_.IsLeftButtonJustReleased()) {
            new_pose = MovePoseByMouseDisplacement(real_object_pose_,
                                                   mouse_tracker_.DisplacementWhileLeftHolded());
        } else {
            new_pose = showing_pose_now;
        }
        real_object_pose_ = new_pose;
    }

    world.MoveObject(0, new_pose);
    return world.GetCamera(0);
}

}  // namespace detail

}  // namespace application
}  // namespace project

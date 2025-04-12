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
                                           geometry::Position{geometry::Point3d{0, 5, -40}}});

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

    world_.AddObject(coffee, geometry::Pose{geometry::Rotation(),
                                            geometry::Position{geometry::Vector3d{0, -0.4, -0.8}}});

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

geometry::Coordinate InteractiveProcessor::ConvertToGeometryCoordinate(int coord) {
    return static_cast<geometry::Coordinate>(coord);
}

geometry::Rotation InteractiveProcessor::MoveRotationByMouseDisplacement(
    geometry::Rotation rotation, sf::Vector2i displacement) {
    static constexpr int pixels_per_radian = 300;
    rotation.rot_matrix = geometry::Rotation::ByAngles(
                              -ConvertToGeometryCoordinate(displacement.x) / pixels_per_radian, 0,
                              ConvertToGeometryCoordinate(displacement.y) / pixels_per_radian)
                              .rot_matrix *
                          rotation.rot_matrix;

    return rotation;
}

geometry::Position InteractiveProcessor::MovePositionByArrows(geometry::Position position,
                                                              bool left_holded, bool right_holded,
                                                              bool up_holded, bool down_holded,
                                                              bool forward_holded, bool back_holded,
                                                              double delta_time) {
    static constexpr geometry::Coordinate distance_per_second = 0.1;
    geometry::Coordinate distance_to_move = distance_per_second * delta_time;
    if (left_holded && !right_holded) {
        position.pos_vector.x() -= distance_to_move;
    } else if (right_holded && !left_holded) {
        position.pos_vector.x() += distance_to_move;
    }

    if (down_holded && !up_holded) {
        position.pos_vector.y() -= distance_to_move;
    } else if (up_holded && !down_holded) {
        position.pos_vector.y() += distance_to_move;
    }

    if (forward_holded&& !back_holded) {
        position.pos_vector.z() -= distance_to_move;
    } else if (back_holded && !forward_holded) {
        position.pos_vector.z() += distance_to_move;
    }

    return position;
}

const kernel::PosedCamera& InteractiveProcessor::operator()(kernel::World& world) {
    mouse_tracker_.Frame();
    left_arrow_tracker_.Frame();
    right_arrow_tracker_.Frame();
    up_arrow_tracker_.Frame();
    down_arrow_tracker_.Frame();
    w_tracker_.Frame();
    s_tracker_.Frame();
    double delta_time = delta_timer_.TimeInSeconds();
    delta_timer_.Reset();

    auto showing_pose_now = geometry::Pose(world.GetObject(0));

    geometry::Rotation new_rot;
    if (mouse_tracker_.IsLeftButtonHolded()) {
        new_rot = MoveRotationByMouseDisplacement(real_object_rotation_,
                                                  mouse_tracker_.DisplacementWhileLeftHolded());
    } else {
        if (mouse_tracker_.IsLeftButtonJustReleased()) {
            new_rot = MoveRotationByMouseDisplacement(real_object_rotation_,
                                                      mouse_tracker_.DisplacementWhileLeftHolded());
        } else {
            new_rot = showing_pose_now;
        }
        real_object_rotation_ = new_rot;
    }

    geometry::Position new_position = MovePositionByArrows(
        showing_pose_now, left_arrow_tracker_.IsHolded(), right_arrow_tracker_.IsHolded(),
        up_arrow_tracker_.IsHolded(), down_arrow_tracker_.IsHolded(), w_tracker_.IsHolded(),
        s_tracker_.IsHolded(), delta_time);

    world.MoveObject(0, {new_rot, new_position});

    return world.GetCamera(0);
}

}  // namespace detail

}  // namespace application
}  // namespace project

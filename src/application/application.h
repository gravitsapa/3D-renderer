#pragma once

#include <string>
#include <renderer.h>
#include <SFML/Graphics.hpp>
#include <functional>
#include <point.h>
#include <mouse_tracker.h>

namespace project {
namespace application {

class Application {
public:
    Application(kernel::Height screen_height, kernel::Width screen_width, std::string scene_name);

    void Run();

private:
    void LoadEmptyScene();
    void LoadSceneWithHouse();
    void LoadSceneWithCoffee();
    void LoadSceneWithChess();
    void ShowScreen();

    sf::RenderWindow window_;
    kernel::World world_;
    kernel::Screen screen_;
    kernel::Renderer renderer_;
    std::function<const kernel::PosedCamera&(kernel::World&)> frame_processor_;
};

namespace detail {
struct CameraPlaneSize {
    geometry::Coordinate h;
    geometry::Coordinate w;
};

CameraPlaneSize ExpandSizeAccordingToResolution(CameraPlaneSize size, kernel::Height height,
                                                kernel::Width width);

// const kernel::PosedCamera& RotateProcessor(kernel::World& world);

class InteractiveProcessor {
public:
    const kernel::PosedCamera& operator()(kernel::World& world);

private:
    geometry::Coordinate ConvertToGeometryCoordinate(int coord);
    geometry::Pose MovePoseByMouseDisplacement(geometry::Pose pose, sf::Vector2i displacement);

    MouseTracker mouse_tracker_;
    geometry::Pose real_object_pose_;
};

}  // namespace detail

}  // namespace application
}  // namespace project

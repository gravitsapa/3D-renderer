#pragma once

#include <string>
#include <renderer.h>
#include <SFML/Graphics.hpp>
#include <functional>
#include <point.h>
#include <mouse_tracker.h>
#include <button_tracker.h>
#include <timer.h>

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

class InteractiveProcessor {
public:
    const kernel::PosedCamera& operator()(kernel::World& world);

private:
    geometry::Coordinate ConvertToGeometryCoordinate(int coord);
    geometry::Rotation MoveRotationByMouseDisplacement(geometry::Rotation rotation,
                                                       sf::Vector2i displacement);
    geometry::Position MovePositionByArrows(geometry::Position position, bool left_holded,
                                            bool right_holded, bool up_holded, bool down_holded,
                                            bool forward_holded, bool back_holded,
                                            double delta_time);

    Timer delta_timer_;
    MouseTracker mouse_tracker_;
    ButtonTracker left_arrow_tracker_ = ButtonTracker(sf::Keyboard::Key::Left);
    ButtonTracker right_arrow_tracker_ = ButtonTracker(sf::Keyboard::Key::Right);
    ButtonTracker up_arrow_tracker_ = ButtonTracker(sf::Keyboard::Key::Up);
    ButtonTracker down_arrow_tracker_ = ButtonTracker(sf::Keyboard::Key::Down);
    ButtonTracker w_tracker_ = ButtonTracker(sf::Keyboard::Key::W);
    ButtonTracker s_tracker_ = ButtonTracker(sf::Keyboard::Key::S);
    geometry::Rotation real_object_rotation_;
};

}  // namespace detail

}  // namespace application
}  // namespace project

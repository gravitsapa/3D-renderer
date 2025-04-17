#pragma once

#include <SFML/Graphics.hpp>
#include <pose.h>
#include <mouse_tracker.h>
#include <button_tracker.h>
#include <timer.h>
#include <world.h>

namespace project {
namespace application {
namespace detail {

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

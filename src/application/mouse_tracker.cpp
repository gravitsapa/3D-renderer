#include <mouse_tracker.h>
#include <iostream>

namespace project {
namespace application {

void MouseTracker::Frame() {
    bool left_holded_now = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
    left_just_released_ = !left_holded_now && left_holded_;
    sf::Vector2i mouse_position_now = sf::Mouse::getPosition();
    if (left_holded_now && !left_holded_) {
        placement_when_left_pressed_ = mouse_position_now;
    }
    if (left_holded_now || left_just_released_) {
        displacement_while_left_pressed_ = mouse_position_now - placement_when_left_pressed_;
    } else {
        displacement_while_left_pressed_ = {0, 0};
    }

    left_holded_ = left_holded_now;
}

bool MouseTracker::IsLeftButtonHolded() const {
    return left_holded_;
}

bool MouseTracker::IsLeftButtonJustReleased() const {
    return left_just_released_;
}

sf::Vector2i MouseTracker::DisplacementWhileLeftHolded() const {
    return displacement_while_left_pressed_;
}

}  // namespace application
}  // namespace project

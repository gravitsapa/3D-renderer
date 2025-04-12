#include <button_tracker.h>

namespace project {
namespace application {

ButtonTracker::ButtonTracker(sf::Keyboard::Key key) : key_(key) {
}

void ButtonTracker::Frame() {
    bool is_pressed_now = sf::Keyboard::isKeyPressed(key_);
    is_holded_ = is_pressed_now && is_pressed_;
    is_pressed_ = is_pressed_now;
}

bool ButtonTracker::IsPressed() const {
    return is_pressed_;
}

bool ButtonTracker::IsHolded() const {
    return is_holded_;
}

}  // namespace application
}  // namespace project

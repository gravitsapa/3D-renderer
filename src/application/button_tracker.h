#pragma once

#include <SFML/Graphics.hpp>

namespace project {
namespace application {

class ButtonTracker {
public:
    ButtonTracker(sf::Keyboard::Key key);

    void Frame();
    bool IsPressed() const;
    bool IsHolded() const;

private:
    sf::Keyboard::Key key_;
    bool is_pressed_ = false;
    bool is_holded_ = false;
};
}  // namespace application
}  // namespace project

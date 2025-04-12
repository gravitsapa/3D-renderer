#pragma once
#include <SFML/Window/Mouse.hpp>

namespace project {
namespace application {

class MouseTracker {
public:
    void Frame();

    bool IsLeftButtonHolded() const;
    bool IsLeftButtonJustReleased() const;
    sf::Vector2i DisplacementWhileLeftHolded() const;
    
private:
    sf::Vector2i placement_when_left_pressed_;
    sf::Vector2i displacement_while_left_pressed_;
    bool left_holded_ = false;
    bool left_just_released_ = false;
};

}  // namespace application
}  // namespace project

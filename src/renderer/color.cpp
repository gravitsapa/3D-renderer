#include <color.h>
#include <cstdlib>
#include <SFML/Graphics.hpp>

namespace project {

namespace kernel {

sf::Color ConvertToSFMLColor(const Color& color) {
    return sf::Color(color.r, color.g, color.b);
}

Color Color::White() {
    return {kChannelMax, kChannelMax, kChannelMax};
}

Color Color::Red() {
    return {kChannelMax, 0, 0};
}

Color Color::Green() {
    return {0, kChannelMax, 0};
}

Color Color::Blue() {
    return {0, 0, kChannelMax};
}

Color Color::Random() {
    return {static_cast<ChannelType>(std::rand() % kChannelMax),
            static_cast<ChannelType>(std::rand() % kChannelMax),
            static_cast<ChannelType>(std::rand() % kChannelMax)};
}

}  // namespace kernel

}  // namespace project

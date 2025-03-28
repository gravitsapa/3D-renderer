#pragma once

#include <climits>
#include <SFML/Graphics.hpp>

namespace project {

namespace kernel {

struct Color {
    using ChannelType = unsigned char;
    static constexpr ChannelType kChannelMax = UCHAR_MAX;

    static Color White();
    static Color Red();
    static Color Green();
    static Color Blue();
    static Color Random();

    ChannelType r = 0;
    ChannelType g = 0;
    ChannelType b = 0;
};

sf::Color ConvertToSFMLColor(const Color& color);

}  // namespace kernel

}  // namespace project

#pragma once

#include <climits>
#include <SFML/Graphics.hpp>

namespace project {

namespace kernel {

struct Color {
    using ChannelType = double;
    static constexpr ChannelType kChannelMax = 1;

    static Color White();
    static Color Black();
    static Color Red();
    static Color Green();
    static Color Blue();
    static Color Random();

    ChannelType r = 0;
    ChannelType g = 0;
    ChannelType b = 0;
};

Color operator+(const Color& col1, const Color& col2);
Color operator*(const Color& col1, const Color& col2);
Color operator*(const Color& col1, Color::ChannelType factor);

using ChannelChar = unsigned char;
constexpr ChannelChar channel_char_max = UCHAR_MAX;

Color ColorByChar(ChannelChar r, ChannelChar g, ChannelChar b);
sf::Color ConvertToSFMLColor(const Color& color);

namespace detail {
Color::ChannelType ChannelByChar(ChannelChar alpha);
ChannelChar CharByChannelType(Color::ChannelType alpha);
}  // namespace detail


}  // namespace kernel

}  // namespace project

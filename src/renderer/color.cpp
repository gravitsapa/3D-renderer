#include <color.h>
#include <cstdlib>
#include <SFML/Graphics.hpp>

namespace project {

namespace kernel {

Color Color::White() {
    return {kChannelMax, kChannelMax, kChannelMax};
}

Color Color::Black() {
    return {0, 0, 0};
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

ChannelType ChannelByChar(ChannelChar alpha) {
    return static_cast<ChannelType>(alpha) / channel_char_max;
}
ChannelChar CharByChannelType(ChannelType alpha) {
    return static_cast<ChannelChar>(std::min(Color::kChannelMax, alpha) * channel_char_max);
}

Color ColorByChar(ChannelChar r, ChannelChar g, ChannelChar b) {
    return Color{ChannelByChar(r), ChannelByChar(g), ChannelByChar(b)};
}
sf::Color ConvertToSFMLColor(const Color& color) {
    return sf::Color(CharByChannelType(color.r), CharByChannelType(color.g),
                     CharByChannelType(color.b));
}

Color operator+(const Color& col1, const Color& col2) {
    return Color{col1.r + col2.r, col1.g + col2.g, col1.b + col2.b};
}
Color operator*(const Color& col1, const Color& col2) {
    return Color{col1.r * col2.r, col1.g * col2.g, col1.b * col2.b};
}

Color operator*(const Color& col1, ChannelType factor) {
    return {col1.r * factor, col1.g * factor, col1.b * factor};
}

Color Color::Random() {
    return ColorByChar(static_cast<ChannelType>(std::rand() % channel_char_max),
            static_cast<ChannelType>(std::rand() % channel_char_max),
            static_cast<ChannelType>(std::rand() % channel_char_max));
}

}  // namespace kernel

}  // namespace project

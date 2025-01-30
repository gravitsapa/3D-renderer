#pragma once

#include <climits>

namespace geometry {

struct Color {
    using ChannelType = unsigned char;
    static constexpr ChannelType kChannelMax = UCHAR_MAX;

    ChannelType r = 0, g = 0, b = 0;

    static Color White();

    static Color Red();

    static Color Green();

    static Color Blue();

    static Color Random();
};

}  // namespace geometry
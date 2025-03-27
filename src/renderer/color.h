#pragma once

#include <climits>

namespace project {

namespace kernel {

struct Color {
    using ChannelType = unsigned char;
    static constexpr ChannelType kChannelMax = UCHAR_MAX;

    ChannelType r = 0;
    ChannelType g = 0;
    ChannelType b = 0;

    static Color White();
    static Color Red();
    static Color Green();
    static Color Blue();
    static Color Random();
};

}  // namespace kernel

}  // namespace project

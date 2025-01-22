#pragma once

#include <climits>

struct Color {
    using ChannelType = unsigned char;
    static constexpr ChannelType kChannelMax = UCHAR_MAX;

    ChannelType r_ = 0, g_ = 0, b_ = 0;

    static constexpr Color White() {
        return { kChannelMax, kChannelMax, kChannelMax };
    }

    static constexpr Color Red() {
        return { kChannelMax, 0, 0 };
    }

    static constexpr Color Green() {
        return { 0, kChannelMax, 0 };
    }

    static constexpr Color Blue() {
        return { 0, 0, kChannelMax };
    }

    static constexpr Color Random() {
        return { rand() % kChannelMax, rand() % kChannelMax, rand() % kChannelMax };
    }
};
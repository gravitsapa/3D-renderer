#pragma once

struct Color {
    using ChannelType = unsigned char;

    ChannelType r_ = 0, g_ = 0, b_ = 0;
};
#pragma once

#include <vector>
#include <color.h>
#include <orientation.h>
#include <table.h>
#include <SFML/Graphics.hpp>

namespace project {
namespace kernel {

class Screen {
public:
    Screen(Height h, Width w);

    Height GetHeight();
    Width GetWidth();
    void SetPixel(Height y, Width x, Color col);
    void Fill(Color col);

    sf::Image ConvertToImage();

private:
    structures::Table<Color> data_;
};

}  // namespace kernel
}  // namespace project

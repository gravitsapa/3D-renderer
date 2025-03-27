#pragma once

#include <vector>
#include <color.h>
#include <orientation.h>
#include <tgaimage.h>

namespace renderer {

using namespace geometry;

class Screen {
public:
    Screen(Height h, Width w);

    TGAImage ConvertToTGA();

    Height GetHeight();

    Width GetWidth();

    void SetPixel(Height y, Width x, Color col);

private:
    std::vector<std::vector<Color>> matrix_;
};

}  // namespace renderer

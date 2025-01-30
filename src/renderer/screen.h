#pragma once

#include <vector>
#include <color.h>
#include <orientation.h>

namespace renderer {

using namespace geometry;

class Screen {
public:
    void SetSize(Height h, Width w);

    Height GetHeight();

    Width GetWidth();

    std::vector<std::vector<Color>>& GetMatrix();

private:
    std::vector<std::vector<Color>> matrix_;
};

}  // namespace scene

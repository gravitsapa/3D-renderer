#pragma once

#include <vector>
#include <color.h>

namespace pipeline {

using namespace geometry;

class Screen {
public:
    void SetSize(size_t h, size_t w);

    size_t GetHeight();

    size_t GetWidth();

    std::vector<std::vector<Color>>& GetMatrix();

private:
    std::vector<std::vector<Color>> matrix_;
};

}  // namespace scene

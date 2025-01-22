#pragma once

#include <vector>
#include <color.h>

class Screen {
public:
    void SetSize(size_t h, size_t w) {
        matrix_.resize(h, std::vector<Color>(w));
    }

    std::vector<std::vector<Color>>& GetMatrix() {
        return matrix_;
    }

private:
    std::vector<std::vector<Color>> matrix_;
};

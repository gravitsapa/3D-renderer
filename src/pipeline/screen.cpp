#include <screen.h>

namespace pipeline {

void Screen::SetSize(size_t h, size_t w) {
    matrix_.resize(h, std::vector<Color>(w));
}

std::vector<std::vector<Color>>& Screen::GetMatrix() {
    return matrix_;
}

size_t Screen::GetHeight() {
    return matrix_.size();
}

size_t Screen::GetWidth() {
    return matrix_[0].size();
}

}  // namespace pipeline
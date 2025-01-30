#include <screen.h>

namespace renderer {

void Screen::SetSize(Height h, Width w) {
    matrix_.resize(h, std::vector<Color>(w));
}

std::vector<std::vector<Color>>& Screen::GetMatrix() {
    return matrix_;
}

Height Screen::GetHeight() {
    return Height(matrix_.size());
}

Width Screen::GetWidth() {
    return Width(matrix_[0].size());
}

}  // namespace pipeline
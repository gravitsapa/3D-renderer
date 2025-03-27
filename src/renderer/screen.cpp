#include <screen.h>
#include <cassert>

namespace renderer {

Screen::Screen(Height h, Width w) : matrix_(h, std::vector<Color>(w)) {
    assert(h > 0 && w > 0 && "Screen size must be positive");
}

TGAImage Screen::ConvertToTGA() {
    TGAImage image(GetWidth(), GetHeight(), TGAImage::RGB);

    for (int y = 0; y < GetHeight(); ++y) {
        for (int x = 0; x < GetWidth(); ++x) {
            const auto& pixel_color = matrix_[y][x];
            image.set(x, y,
                      TGAColor(pixel_color.r, pixel_color.g,
                               pixel_color.b, 255));
        }
    }

    return image;
}

void Screen::SetPixel(Height y, Width x, Color col) {
    matrix_[y][x] = col;
}

Height Screen::GetHeight() {
    return Height(matrix_.size());
}

Width Screen::GetWidth() {
    return Width(matrix_[0].size());
}

}  // namespace renderer

#include <screen.h>
#include <cassert>

namespace project {
namespace kernel {

Screen::Screen(Height h, Width w) : data_(h, w) {
    assert(h > 0 && w > 0 && "Screen size must be positive");
}

TGAImage Screen::ConvertToTGA() {
    TGAImage image(GetWidth(), GetHeight(), TGAImage::RGB);

    for (int y = 0; y < GetHeight(); ++y) {
        for (int x = 0; x < GetWidth(); ++x) {
            const auto& pixel_color = data_.Get(y, x);
            image.set(x, y, TGAColor(pixel_color.r, pixel_color.g, pixel_color.b, 255));
        }
    }

    return image;
}

void Screen::SetPixel(Height y, Width x, Color col) {
    data_.Get(y, x) = col;
}

void Screen::Fill(Color col) {
    data_.Fill(col);
}

Height Screen::GetHeight() {
    return Height(data_.GetHeight());
}

Width Screen::GetWidth() {
    return Width(data_.GetWidth());
}

}  // namespace kernel
}  // namespace project

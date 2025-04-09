#include <screen.h>
#include <cassert>
#include <SFML/Graphics.hpp>

namespace project {
namespace kernel {

Screen::Screen(Height h, Width w) : data_(h, w) {
    assert(h > 0 && w > 0 && "Screen size must be positive");
}

sf::Image Screen::ConvertToImage() {
    sf::Image image({GetWidth(), GetHeight()});
    for (int x = 0; x < GetWidth(); ++x) {
        for (int y = 0; y < GetHeight(); ++y) {
            image.setPixel({x, GetHeight() - y - 1}, ConvertToSFMLColor(data_.Get(y, x)));
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

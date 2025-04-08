#include <texture.h>

namespace project {
namespace kernel {

Texture::Texture() : data_(1, 1, Color::Red()) {
}

int Texture::ConvertToIndexH(TextureCoordinate h) const {
    assert(0 <= h && h <= 1);
    return static_cast<int>(h * (data_.GetHeight() - 1));
}

int Texture::ConvertToIndexW(TextureCoordinate w) const {
    assert(0 <= w && w <= 1);
    return static_cast<int>(w * (data_.GetWidth() - 1));
}

Color Texture::GetPixelColor(const TextureCoordinates& coord) const {
    assert(data_.GetHeight() > 0 && data_.GetWidth() > 0);
    return data_.Get(ConvertToIndexH(coord.x()), ConvertToIndexW(coord.y()));
}

}  // namespace kernel
}  // namespace project

#pragma once

#include <string>
#include <table.h>
#include <color.h>

namespace project {
namespace kernel {

using TextureCoordinate = double;

struct TextureCoordinates {
    TextureCoordinate h;
    TextureCoordinate w;
};

class Texture {
    friend Texture ReadTextureFromFile(const std::string& filename);

public:
    Texture();

    Color GetPixelColor(TextureCoordinates coord) const;

private:
    int ConvertToIndexH(TextureCoordinate h) const;
    int ConvertToIndexW(TextureCoordinate w) const;

    structures::Table<Color> data_;
};

}  // namespace kernel
}  // namespace project

#pragma once

#include <string>
#include <table.h>
#include <color.h>
#include <Eigen/Dense>

namespace project {
namespace kernel {

using TextureCoordinate = double;
using TextureCoordinates = Eigen::Vector2<TextureCoordinate>;

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

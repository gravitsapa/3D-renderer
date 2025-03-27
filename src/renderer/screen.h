#pragma once

#include <vector>
#include <color.h>
#include <orientation.h>
#include <tgaimage.h>
#include <table.h>

namespace project {
namespace kernel {

class Screen {
public:
    Screen(Height h, Width w);

    Height GetHeight();
    Width GetWidth();
    void SetPixel(Height y, Width x, Color col);

    TGAImage ConvertToTGA();

private:
    structures::Table<Color> data_;
};

}  // namespace kernel
}  // namespace project

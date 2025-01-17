#include <iostream>

#include "world.h"

int main(int argc, char** argv) {
    World world;

    Object basic_triangle;
    basic_triangle.AddPolygon({{0, 0, 0}, {1, 2, 3}, {2, 2, 8}, Color()});

    world.AddObject(basic_triangle);

    std::cout << "Added triangle" << std::endl;
    return 0;
}
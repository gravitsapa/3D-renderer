#include <iostream>

#include <world.h>
#include <pipeline.h>
#include <tgaimage.h>

int main(int argc, char** argv) {
    World world;

    Object basic_triangle;
    basic_triangle.AddPolygon({{0, 40, -10}, {40, 0, -10}, {-20, -20, -10}, Color::Red()});

    world.AddObject(basic_triangle);

    const int H = 70;
    const int W = 100;

    Camera basic_camera = {1, 100, W, W, H, H};
    world.AddCamera(basic_camera);

    Pipeline pipeline;
    pipeline.SetScreenSize(2 * H, 2 * W);
    pipeline.SetWorld(&world);
    pipeline.SetCamera(&world.GetCameras()[0]);
    auto screen = pipeline.Project();

    TGAImage image(2 * W, 2 * H, TGAImage::RGB);

    for (int y = 0; y < 2 * H; ++y) {
        for (int x = 0; x < 2 * W; ++x) {
            image.set(x, y,
                      TGAColor(screen.GetMatrix()[y][x].r_, screen.GetMatrix()[y][x].g_,
                               screen.GetMatrix()[y][x].b_, 255));
        }
    }

    const char* filename = argv[1];
    image.write_tga_file(filename);

    std::cout << "Pipeline done" << std::endl;
    return 0;
}
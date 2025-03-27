#include <application.h>
#include <iostream>
#include <world.h>
#include <renderer.h>
#include <tgaimage.h>

namespace application {
void Application::Run(const char* filename) {

    scene::World world;

    world.AddObject(
        geometry::Object::RectangularСuboid(130, 100, 150),
        {{geometry::Vector3d{0, 0, -200}}, geometry::Rotation::ByAngles(0, 0.75, 0.75)});

    const int H = 400;
    const int W = 600;

    scene::Camera basic_camera = {50, 500, W / 2, W / 2, H / 2, H / 2};
    world.AddCamera(basic_camera);

    renderer::Screen screen;
    screen.SetSize(renderer::Height(H), renderer::Width(W));
    renderer::Renderer renderer;
    screen = renderer.Project(world, world.GetCameras()[0], std::move(screen));

    TGAImage image(W, H, TGAImage::RGB);

    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            image.set(x, y,
                      TGAColor(screen.GetMatrix()[y][x].r, screen.GetMatrix()[y][x].g,
                               screen.GetMatrix()[y][x].b, 255));
        }
    }

    image.write_tga_file(filename);

    std::cout << "Renderer done" << std::endl;
}

}  // namespace application

#include <application.h>
#include <iostream>
#include <world.h>
#include <renderer.h>
#include <tgaimage.h>

namespace project {
Application::Application(const std::string& output_filename, renderer::Height screen_height,
                         renderer::Width screen_width)
    : output_filename_(output_filename),
      screen_height_(screen_height),
      screen_width_(screen_width) {
}

void Application::Run() {
    world_.AddObject(
        geometry::Object::RectangularСuboid(130, 100, 150),
        {{geometry::Vector3d{0, 0, -200}}, geometry::Rotation::ByAngles(0, 0.75, 0.75)});

    scene::Camera basic_camera = {
        50, 500, screen_width_ / 2, screen_width_ / 2, screen_height_ / 2, screen_height_ / 2};
    world_.AddCamera(basic_camera);

    renderer::Screen screen;
    screen.SetSize(screen_height_, screen_width_);
    renderer::Renderer renderer;
    screen = renderer.Project(world_, world_.GetCameras()[0], std::move(screen));

    TGAImage image(screen_width_, screen_height_, TGAImage::RGB);

    for (int y = 0; y < screen_height_; ++y) {
        for (int x = 0; x < screen_width_; ++x) {
            image.set(x, y,
                      TGAColor(screen.GetMatrix()[y][x].r, screen.GetMatrix()[y][x].g,
                               screen.GetMatrix()[y][x].b, 255));
        }
    }

    image.write_tga_file(output_filename_.c_str());

    std::cout << "Renderer done" << std::endl;
}

}  // namespace project

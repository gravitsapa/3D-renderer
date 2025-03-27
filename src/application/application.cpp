#include <application.h>
#include <iostream>
#include <world.h>
#include <renderer.h>
#include <tgaimage.h>

namespace project {
Application::Application(const std::string& output_filename, renderer::Height screen_height,
                         renderer::Width screen_width)
    : output_filename_(output_filename), screen_(screen_height, screen_width) {
    world_.AddObject(
        geometry::Object::RectangularСuboid(130, 100, 150),
        {{geometry::Vector3d{0, 0, -200}}, geometry::Rotation::ByAngles(0, 0.75, 0.75)});

    world_.AddCamera(scene::Camera{50, 500, 200, 200, 300, 300});
}

void Application::Run() {
    renderer::Renderer renderer;
    screen_ = renderer.Project(world_, world_.GetCameras()[0], std::move(screen_));

    TGAImage image = screen_.ConvertToTGA();
    image.write_tga_file(output_filename_.c_str());

    std::cerr << "Renderer done" << std::endl;
}

}  // namespace project

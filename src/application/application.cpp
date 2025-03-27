#include <application.h>
#include <iostream>
#include <world.h>
#include <renderer.h>
#include <tgaimage.h>

namespace project {
namespace application {
Application::Application(const std::string& output_filename, kernel::Height screen_height,
                         kernel::Width screen_width)
    : output_filename_(output_filename), screen_(screen_height, screen_width) {
    world_.AddObject(
        kernel::Object::RectangularСuboid(130, 100, 150),
        geometry::Pose{geometry::Rotation::ByAngles(0, 0.75, 0.75), geometry::Position{geometry::Vector3d{0, 0, -200}}});

    world_.AddCamera(kernel::Camera{50, 500, 200, 200, 300, 300});
}

void Application::Run() {
    kernel::Renderer renderer;
    screen_ = renderer.Project(world_, world_.GetCameras()[0], std::move(screen_));

    TGAImage image = screen_.ConvertToTGA();
    image.write_tga_file(output_filename_.c_str());

    std::cerr << "Renderer done" << std::endl;
}

}  // namespace application
}  // namespace project

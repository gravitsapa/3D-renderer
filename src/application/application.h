#pragma once

#include <string>
#include <renderer.h>
#include <tgaimage.h>

namespace project {

class Application {
public:
    Application(const std::string& output_filename, renderer::Height screen_height,
                renderer::Width screen_width);

    void Run();

private:
    std::string output_filename_;
    scene::World world_;
    renderer::Screen screen_;
};

}  // namespace project

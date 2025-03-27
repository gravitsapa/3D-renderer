#pragma once

#include <string>
#include <renderer.h>

namespace project {

class Application {
public:
    Application(const std::string& output_filename, renderer::Height screen_height,
                renderer::Width screen_width);

    void Run();

private:
    std::string output_filename_;
    scene::World world_;
    renderer::Height screen_height_;
    renderer::Width screen_width_;
};

}  // namespace project

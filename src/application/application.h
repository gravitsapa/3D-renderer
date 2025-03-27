#pragma once

#include <string>
#include <renderer.h>
#include <tgaimage.h>

namespace project {
namespace application {

class Application {
public:
    Application(const std::string& output_filename, kernel::Height screen_height,
                kernel::Width screen_width);

    void Run();

private:
    std::string output_filename_;
    kernel::World world_;
    kernel::Screen screen_;
};

}  // namespace application
}  // namespace project

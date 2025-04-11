#include <fps_counter.h>
#include <iostream>

namespace project {
namespace application {

FpsCounter::FpsCounter(double period_to_print_in_sec)
    : timer_(period_to_print_in_sec * CLOCKS_PER_SEC) {
}

void FpsCounter::Frame() {
    frames_done_++;
    if (timer_.Tick()) {
        double mean_fps = static_cast<double>(frames_done_) / timer_.TimeInSeconds();
        std::cout << "Mean fps: " << mean_fps << std::endl;
    }
}

}  // namespace application
}  // namespace project

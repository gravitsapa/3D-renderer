#pragma once

#include <timer.h>

namespace project {
namespace application {

class FpsCounter {
public:
    FpsCounter(double period_to_print_in_sec);

    void Frame();
private:
    Timer timer_;
    int frames_done_ = 0;
};
}  // namespace application
}  // namespace project

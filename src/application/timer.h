#pragma once

#include <ctime>

namespace project {
namespace application {

class Timer {
public:
    Timer();
    Timer(std::clock_t tick_period);

    double TimeInSeconds();
    void StartTicking(std::clock_t tick_period);
    bool Tick();
    void StopTicking();

private:
    std::clock_t start_clocks_;
    std::clock_t start_tick_clocks_;
    std::clock_t tick_period_;
    clock_t ticks_done_;
    bool ticking_;
};

}  // namespace application
}  // namespace project

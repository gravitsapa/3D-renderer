#include <timer.h>
#include <cassert>

namespace project {
namespace application {
Timer::Timer()
    : start_clocks_(std::clock()),
      start_tick_clocks_(0),
      tick_period_(0),
      ticks_done_(0),
      ticking_(false) {
}

Timer::Timer(std::clock_t tick_period)
    : start_clocks_(std::clock()),
      start_tick_clocks_(std::clock()),
      tick_period_(tick_period),
      ticks_done_(0),
      ticking_(true) {
    assert(tick_period > 0);
}

double Timer::TimeInSeconds() {
    return (double)(clock() - start_clocks_) / CLOCKS_PER_SEC;
}

void Timer::StartTicking(std::clock_t tick_period) {
    assert(tick_period > 0);
    start_tick_clocks_ = clock();
    tick_period_ = tick_period;
    ticks_done_ = 0;
    ticking_ = true;
}

bool Timer::Tick() {
    if (!ticking_) {
        return false;
    }

    if ((clock() - start_tick_clocks_) / tick_period_ > ticks_done_) {
        ticks_done_++;
        return true;
    }

    return false;
}

void Timer::StopTicking() {
    ticking_ = false;
}

void Timer::Reset() {
    start_clocks_ = std::clock();
    if (ticking_) {
        start_tick_clocks_ = start_clocks_;
        ticks_done_ = 0;
    }
}

}  // namespace application
}  // namespace project

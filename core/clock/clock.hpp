#pragma once

#include <chrono>
#include <cstdint>
#include <functional>
#include <type_traits>

#include "core/platform.hpp"
#include "core/sdk.hpp"
#include "core/sys_defs.hpp"

using namespace std::chrono_literals;

namespace ymd {
using Milliseconds = std::chrono::milliseconds;
using Microseconds = std::chrono::microseconds;
using Nanoseconds = std::chrono::nanoseconds;
}  // namespace ymd

namespace ymd::clock {

Milliseconds millis(void);
Microseconds micros(void);
Nanoseconds nanos(void);

void delay(Milliseconds ms);
void delay(Microseconds us);
void delay(Nanoseconds ns);

namespace details {
template <typename T>
struct ClockCtrp {};
}  // namespace details

struct SystemClock final : public details::ClockCtrp<SystemClock> {
    using rep = int64_t;
    using period = std::ratio<1, 1000>;
    using duration = std::chrono::duration<rep, period>;
    using time_point = std::chrono::time_point<SystemClock>;

    static constexpr bool is_monotonic = true;
    static constexpr bool is_steady = true;

    static constexpr bool is_free_running = true;
    static constexpr bool is_stopped_in_halting_debug_mode = true;
    static constexpr bool is_always_enabled = true;
    static constexpr bool is_nmi_safe = false;

    static time_point now() noexcept { return time_point(duration(millis())); }
};

inline auto now() { return SystemClock::now(); }

}  // namespace ymd::clock

namespace ymd {
void bindSystickCb(std::is_function<void(void)> &&cb);
}

extern "C" {
void Systick_Init(void);
__interrupt void SysTick_Handler(void);
}
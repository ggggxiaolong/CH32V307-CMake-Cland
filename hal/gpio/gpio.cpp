#include "gpio.hpp"

#include <cstdint>

#include "core/math_defs.hpp"
#include "core/sdk.hpp"
#include "gpio_port_intf.hpp"

using namespace ymd::hal;

void Gpio::set_mode(const GpioMode mode) {
    if (not is_valid()) return;
    const auto ctz_pin = CTZ(uint16_t(pin_));
    auto& pin_cfg = (ctz_pin >= 8) ? (instance_->CFGHR) : (instance_->CFGLR);
    uint32_t tempreg = pin_cfg;
    const auto shifts = ((ctz_pin % 8) * 4);
    tempreg &= ~(0xF << shifts);
    tempreg |= ((uint8_t)mode << shifts);
    pin_cfg = tempreg;

    if (mode == GpioMode::InPullUp) {
        instance_->OUTDR |= uint16_t(pin_);
    } else if (mode == GpioMode::InPullDown) {
        instance_->OUTDR &= ~uint16_t(pin_);
    }
}

Gpio& Gpio::null() {
    static Gpio null_gpio = Gpio(GPIOD, PinSource::None);
    return null_gpio;
}

namespace ymd::hal {
Gpio& NullGpio = Gpio::null();
}
#pragma once

#include <cstdint>

#include "core/constants/enums.hpp"
#include "core/sys_defs.hpp"
#include "gpio_intf.hpp"
#include "gpio_port_intf.hpp"
#include "hal/gpio/gpio_utils.hpp"

namespace ymd::hal {

class Gpio;

class VGpio final : public GpioIntf {
   protected:
    GpioPortIntf& port_;
    int8_t pin_index_;
    static GpioPortIntf& from_gpio_typedef_to_port(uint32_t base);

   public:
    VGpio(const VGpio&) = delete;
    VGpio(VGpio&&) = delete;
    using GpioIntf::operator=;

    VGpio(const Gpio& gpio);
    VGpio(GpioPortIntf& port, int8_t pin_index);
    VGpio(GpioPortIntf& port, const PinSource pin);

    __fast_inline void set() { port_.set_by_index(pin_index_); }
    __fast_inline void clr() { port_.clr_by_index(pin_index_); }
    __fast_inline void write(const BoolLevel level) { port_.write_by_index(pin_index_, level); }
    __fast_inline BoolLevel read() const { return port_.read_by_index(pin_index_); }
    __fast_inline int8_t index() const { return pin_index_; }
    void set_mode(const GpioMode mode) { port_.set_mode(pin_index_, mode); }
};
}  // namespace ymd::hal
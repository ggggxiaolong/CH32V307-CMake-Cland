#pragma once

#include <cstdint>

#include "core/constants/enums.hpp"
#include "gpio_utils.hpp"

namespace ymd::hal {
class GpioIntf {
   public:
    GpioIntf() = default;
    virtual ~GpioIntf() = default;
    GpioIntf(const hal::GpioIntf& other) = delete;
    GpioIntf(hal::GpioIntf&& other) = delete;

    virtual void set() = 0;
    virtual void clr() = 0;
    virtual void write(const BoolLevel level) = 0;
    virtual BoolLevel read() const = 0;
    virtual int8_t index() const = 0;
    virtual void set_mode(const GpioMode mode) = 0;

    BoolLevel toggle() {
        const BoolLevel val = not read();
        write(val);
        return val;
    }

    bool is_valid() const { return index() >= 0; }
    hal::GpioIntf& operator=(const BoolLevel level) {
        write(level);
        return *this;
    }

    hal::GpioIntf& operator=(const hal::GpioIntf& other) {
        write(other.read());
        return *this;
    }

    operator BoolLevel() const { return (this->read()); }

    hal::GpioIntf& outpp() {
        set_mode(GpioMode::OutPushPull);
        return *this;
    }
    hal::GpioIntf& outod() {
        set_mode(GpioMode::OutOpenDrain);
        return *this;
    }
    hal::GpioIntf& afpp() {
        set_mode(GpioMode::OutAfPushPull);
        return *this;
    }
    hal::GpioIntf& afod() {
        set_mode(GpioMode::OutAfOpenDrain);
        return *this;
    }
    hal::GpioIntf& outpp(const BoolLevel initial_state) {
        set_mode(GpioMode::OutPushPull);
        write(initial_state);
        return *this;
    }
    hal::GpioIntf& outod(const BoolLevel initial_state) {
        set_mode(GpioMode::OutOpenDrain);
        write(initial_state);
        return *this;
    }
    hal::GpioIntf& afpp(const BoolLevel initial_state) {
        set_mode(GpioMode::OutAfPushPull);
        write(initial_state);
        return *this;
    }
    hal::GpioIntf& afod(const BoolLevel initial_state) {
        set_mode(GpioMode::OutAfOpenDrain);
        write(initial_state);
        return *this;
    }

    hal::GpioIntf& inanan() {
        set_mode(GpioMode::InAnalog);
        return *this;
    }

    hal::GpioIntf& inflt() {
        set_mode(GpioMode::InFloating);
        return *this;
    }

    hal::GpioIntf& inpu() {
        set_mode(GpioMode::InPullUp);
        return *this;
    }

    hal::GpioIntf& inpd() {
        set_mode(GpioMode::InPullDown);
        return *this;
    }
};
}  // namespace ymd::hal
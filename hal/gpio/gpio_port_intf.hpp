#pragma once

#include "core/constants/enums.hpp"
#include "core/sys_defs.hpp"
#include "gpio_utils.hpp"

namespace ymd::hal {

class GpioPortIntf {
   public:
    virtual void write_by_index(const size_t index, const BoolLevel level);
    virtual BoolLevel read_by_index(const size_t index);
    virtual void set_by_index(const size_t index);
    virtual void clr_by_index(const size_t index);
    virtual void set_by_mask(const PinMask mask);
    virtual void clr_by_mask(const PinMask mask);
    virtual void write_by_mask(const PinMask mask);
    virtual PinMask read_mask() const;
    virtual void set_mode(const size_t index, const GpioMode mode);

    void set_pin(const PinSource pin) { set_by_mask(PinMask(pin)); }
    void clr_pin(const PinSource pin) { clr_by_mask(PinMask(pin)); }
};

__fast_inline void GpioPortIntf::set_by_index(const size_t index) {
    if (index < 0) return;
    set_by_mask(PinMask::from_index(index));
}

__fast_inline void GpioPortIntf::clr_by_index(const size_t index) {
    if (index < 0) return;
    clr_by_mask(PinMask::from_index(index));
}

__fast_inline void GpioPortIntf::write_by_index(const size_t index, const BoolLevel level) {
    if (index < 0) return;
    const auto mask = PinMask::from_index(index);
    if (level == HIGH) {
        set_by_mask(mask);
    } else {
        clr_by_mask(mask);
    }
}

__fast_inline BoolLevel GpioPortIntf::read_by_index(const size_t index) { return this->read_mask().test(index) ? HIGH : LOW; }
}  // namespace ymd::hal
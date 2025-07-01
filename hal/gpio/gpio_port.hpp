#pragma once

#include <sys/cdefs.h>

#include <cstddef>
#include <cstdint>

#include "core/math_defs.hpp"
#include "gpio.hpp"
#include "gpio_port_intf.hpp"
#include "hal/gpio/gpio_utils.hpp"
#include "sdk/Peripheral/inc/ch32v30x.h"

namespace ymd::hal {

class GpioPort final : public GpioPortIntf {
   protected:
    GPIO_TypeDef* instance;
    Gpio channel[16];
    friend class Gpio;

   public:
    // clang-format off
    GpioPort(GPIO_TypeDef* _instance)
        : instance(_instance),
          channel{
              Gpio(instance, PinSource::_0),  
              Gpio(instance, PinSource::_1),  
              Gpio(instance, PinSource::_2),  
              Gpio(instance, PinSource::_3),
              Gpio(instance, PinSource::_4),  
              Gpio(instance, PinSource::_5),  
              Gpio(instance, PinSource::_6),  
              Gpio(instance, PinSource::_7),
              Gpio(instance, PinSource::_8),  
              Gpio(instance, PinSource::_9),  
              Gpio(instance, PinSource::_10), 
              Gpio(instance, PinSource::_11),
              Gpio(instance, PinSource::_12), 
              Gpio(instance, PinSource::_13), 
              Gpio(instance, PinSource::_14), 
              Gpio(instance, PinSource::_15),
          } {}
    // clang-format on
    void init();
    void enableRcc(const Enable en = EN);

    __inline void write_by_index(const size_t index, const BoolLevel level) override;
    __inline void set_by_mask(const PinMask mask) override;
    __inline void clr_by_mask(const PinMask mask) override;
    void set_mode(const size_t index, const GpioMode mode) override;

    __inline void write_by_mask(const PinMask mask) override { instance->OUTDR = mask.as_u16(); }
    __inline PinMask read_mask() const override { return PinMask(instance->INDR); }
    Gpio& operator[](const size_t index) { return channel[index & 0b1111]; }

    Gpio& operator[](const PinSource pin) {
        if (pin != PinSource::None) {
            return channel[CTZ(uint16_t(pin)) & 0b1111];
        } else
            return Gpio::null();
    }
};

__inline void GpioPort::write_by_index(const size_t index, const BoolLevel level) {
    const auto mask = PinMask::from_index(index);
    if (level == HIGH) {
        set_by_mask(mask);
    } else {
        clr_by_mask(mask);
    }
}

__inline void GpioPort::set_by_mask(const PinMask mask) { instance->BSHR = mask.as_u16(); }

__inline void GpioPort::clr_by_mask(const PinMask mask) { instance->BCR = mask.as_u16(); }

#ifdef ENABLE_GPIOA
extern GpioPort portA;
template <size_t I>
    requires(I < 16)
static constexpr Gpio& PA() {
    return portA[I];
}
#endif

#ifdef ENABLE_GPIOB
extern GpioPort portB;
template <size_t I>
    requires(I < 16)
static constexpr Gpio& PB() {
    return portB[I];
}
#endif

#ifdef ENABLE_GPIOC
extern GpioPort portC;
template <size_t I>
    requires(I < 16)
static constexpr Gpio& PC() {
    return portC[I];
}
#endif

#ifdef ENABLE_GPIOD
extern GpioPort portD;
template <size_t I>
    requires(I < 16)
static constexpr Gpio& PD() {
    return portD[I];
}
#endif

#ifdef ENABLE_GPIOE
extern GpioPort portE;
template <size_t I>
    requires(I < 16)
static constexpr Gpio& PE() {
    return portE[I];
}
#endif
}  // namespace ymd::hal
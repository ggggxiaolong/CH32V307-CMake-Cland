#pragma once

#include <cstdint>

#include "core/constants/enums.hpp"
#include "core/math_defs.hpp"
#include "core/sdk.hpp"
#include "core/sys_defs.hpp"
#include "gpio_intf.hpp"
#include "gpio_tag.hpp"
#include "hal/gpio/gpio_utils.hpp"
#include "sdk/Peripheral/inc/ch32v30x.h"

namespace ymd::hal {

class Exti;

class Gpio final : public GpioIntf {
   protected:
    GPIO_TypeDef *instance_;
    const PinSource pin_;

    Gpio(GPIO_TypeDef *instance, PinSource pin)
        : instance_(instance),
#if defined(CH32V20X) || defined(CH32V30X)
          pin_(PinSource((instance_ == GPIOC) &&
                                 (((*reinterpret_cast<uint32_t *>(0x40022030) & 0x0F000000) == 0)  // MCU version for wch mcu, see wch sdk
                                  )
                             ? uint16_t(uint16_t(pin) >> 13)
                             : uint16_t(pin)))
#elif defined(USE_STM32_HAL_LIB)
          pin_(pin)
#endif
    {};

    friend class VGpio;
    friend class ExtiChannel;
    friend class GpioPort;

   public:
    using GpioIntf::operator=;
    Gpio(const Gpio &) = delete;
    Gpio(Gpio &&) = delete;
    ~Gpio() {};

    static Gpio &null();

    __fast_inline void set() { instance_->BSHR = uint16_t(pin_); }

    __fast_inline void clr() { instance_->BCR = uint16_t(pin_); }

    // BSHR的寄存器在BCR前 {1->BSHR; 0->BCR} 使用逻辑操作而非判断以提高速度
    __fast_inline void write(const BoolLevel level) { *(&instance_->BCR - int(level.to_bool())) = uint16_t(pin_); }

    __fast_inline BoolLevel read() const { return BoolLevel::from(instance_->INDR & uint16_t(pin_)); }

    hal::Gpio &operator=(const BoolLevel level) {
        write(level);
        return *this;
    }

    hal::Gpio &operator=(const hal::Gpio &other) {
        write(other.read());
        return *this;
    }

    virtual void set_mode(const GpioMode mode);

    __fast_inline GPIO_TypeDef *inst() const { return instance_; }

    __fast_inline int8_t index() const {
        if (likely(uint16_t(pin_))) {
            return CTZ(uint16_t(pin_));
        } else {
            return -1;
        }
    }

    template <hal::GpioTags::PortSource port_source, hal::GpioTags::PinSource pin_source>
    static constexpr Gpio reflect() {
        // GPIO_TypeDef *_instance = GPIOC;
        const auto pin_ = PinSource(1 << uint8_t(pin_source));
        switch (port_source) {
            case hal::GpioTags::PortSource::PA:
                return Gpio(GPIOA, pin_);
            case hal::GpioTags::PortSource::PB:
                return Gpio(GPIOB, pin_);
            case hal::GpioTags::PortSource::PC:
                return Gpio(GPIOC, pin_);
            case hal::GpioTags::PortSource::PD:
                return Gpio(GPIOD, pin_);
            case hal::GpioTags::PortSource::PE:
                return Gpio(GPIOE, pin_);
            default:
                return Gpio(GPIOC, pin_);
        }
    }

    constexpr PinSource pin() const { return pin_; }

    constexpr PortSource port() const {
        const auto base = reinterpret_cast<uint32_t>(instance_);
        switch (base) {
            default:
            case GPIOA_BASE:
                return PortSource::PA;
            case GPIOB_BASE:
                return PortSource::PB;
            case GPIOC_BASE:
                return PortSource::PC;
            case GPIOD_BASE:
                return PortSource::PD;
            case GPIOE_BASE:
                return PortSource::PE;
        }
    }
};

extern Gpio &NullGpio;
}  // namespace ymd::hal
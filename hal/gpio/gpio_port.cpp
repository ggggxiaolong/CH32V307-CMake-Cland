#include "gpio_port.hpp"

#include <bit>
#include <cstdint>

#include "core/constants/enums.hpp"
#include "gpio.hpp"
#include "hal/gpio/gpio_utils.hpp"
#include "sdk/Core/core_riscv.h"
#include "sdk/Peripheral/inc/ch32v30x.h"

using namespace ymd::hal;

void GpioPort::set_mode(const size_t index, const GpioMode mode) {
    Gpio gpio = Gpio(instance, std::bit_cast<PinSource>(uint16_t(1 << index)));
    gpio.set_mode(mode);
}

void GpioPort::enableRcc(const Enable en) {
    auto state = en == EN ? FunctionalState::ENABLE : FunctionalState::DISABLE;
    switch ((uint32_t)instance) {
#ifdef ENABLE_GPIOA
        case GPIOA_BASE:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, state);
#endif
#ifdef ENABLE_GPIOB
        case GPIOB_BASE:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, state);
#endif
#ifdef ENABLE_GPIOC
        case GPIOC_BASE:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, state);
#endif
#ifdef ENABLE_GPIOD
        case GPIOD_BASE:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, state);
#endif
#ifdef ENABLE_GPIOE
        case GPIOE_BASE:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, state);
#endif
        default:
            break;
    }
}

void GpioPort::init() { enableRcc(); }

namespace ymd::hal {

#ifdef ENABLE_GPIOA
GpioPort portA(GPIOA);
#endif

#ifdef ENABLE_GPIOB
GpioPort portB(GPIOB);
#endif

#ifdef ENABLE_GPIOC
GpioPort portC(GPIOC);
#endif

#ifdef ENABLE_GPIOD
GpioPort portD(GPIOD);
#endif

#ifdef ENABLE_GPIOE
GpioPort portE(GPIOE);
#endif
}  // namespace ymd::hal
#include "vgpio.hpp"

#include <cstdint>

#include "hal/gpio/gpio.hpp"
#include "hal/gpio/gpio_port.hpp"
#include "hal/gpio/gpio_port_intf.hpp"

using namespace ymd::hal;

VGpio::VGpio(const Gpio& gpio) : port_(from_gpio_typedef_to_port(uint32_t(gpio.inst()))), pin_index_(gpio.index()) {}

GpioPortIntf& VGpio::from_gpio_typedef_to_port(uint32_t base) {
    switch (base) {
        default:
#ifdef ENABLE_GPIOA
        case GPIOA_BASE:
            return portA;
#endif
#ifdef ENABLE_GPIOB
        case GPIOB_BASE:
            return portB;
#endif
#ifdef ENABLE_GPIOC
        case GPIOC_BASE:
            return portC;
#endif
#ifdef ENABLE_GPIOD
        case GPIOD_BASE:
            return portD;
#endif
#ifdef ENABLE_GPIOE
        case GPIOE_BASE:
            return portE;
#endif
    }
}
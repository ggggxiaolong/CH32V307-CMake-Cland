#include "timer_oc.hpp"

#include <cstdint>

#include "core/constants/enums.hpp"
#include "core/system.hpp"
#include "hal/gpio/gpio.hpp"
#include "hal/gpio/gpio_port.hpp"
#include "hal/timer/timer_utils.hpp"
#include "sdk/Peripheral/inc/ch32v30x.h"
#include "sdk/Peripheral/inc/ch32v30x_tim.h"

using namespace ymd;
using namespace ymd::hal;

void TimerOut::install_to_pin(const Enable en) {
    Gpio &io = internal::get_pin(instance, idx_);
    if (en.to_bool()) {
        io.afpp();
    } else {
        io.inflt();
    }
}

void TimerOut::set_valid_level(const BoolLevel level) {
    if (level == LOW) {
        instance->CCER |= (1 << ((uint8_t)idx_ * 2 + 1));
    } else {
        instance->CCER &= ~(1 << ((uint8_t)idx_ * 2 + 1));
    }
}

void TimerOC::init(const TimerOcPwmConfig &cfg) {
    set_oc_mode(cfg.oc_mode);
    enable_cvr_sync(cfg.cvr_sync_en);
    set_valid_level(cfg.valid_level);
    install_to_pin(cfg.install_en);
    enable_output(cfg.out_en);
}

void TimerOCN::init(const TimerOcnPwmConfig &cfg) {
    install_to_pin(cfg.install_en);
    enable_output(cfg.out_en);
}

void TimerOC::set_oc_mode(const TimerOC::Mode mode) {
    using enum ChannelIndex;
    const uint8_t raw_code = uint8_t(mode) << 4;
    switch (idx_) {
        case CH1: {
            uint16_t tmp_ccmrx = instance->CHCTLR1;
            const uint16_t m_code = TIM_OC1M;
            const uint16_t s_code = TIM_CC1S;
            tmp_ccmrx &= ~m_code;
            tmp_ccmrx &= ~s_code;
            tmp_ccmrx |= uint16_t(raw_code);
            instance->CHCTLR1 = tmp_ccmrx;
            break;
        }
        case CH2: {
            uint16_t tmp_ccmrx = instance->CHCTLR1;
            const uint16_t m_code = TIM_OC2M;
            const uint16_t s_code = TIM_CC2S;
            tmp_ccmrx &= ~m_code;
            tmp_ccmrx &= ~s_code;
            tmp_ccmrx |= uint16_t(raw_code) << 8;
            instance->CHCTLR2 = tmp_ccmrx;
            break;
        }
        case CH3: {
            uint16_t tmp_ccmrx = instance->CHCTLR2;
            const uint16_t m_code = TIM_OC3M;
            const uint16_t s_code = TIM_CC3S;
            tmp_ccmrx &= ~m_code;
            tmp_ccmrx &= ~s_code;
            tmp_ccmrx |= uint16_t(raw_code);
            instance->CHCTLR2 = tmp_ccmrx;
            break;
        }
        case CH4: {
            uint16_t tmp_ccmrx = instance->CHCTLR2;
            const uint16_t m_code = TIM_OC4M;
            const uint16_t s_code = TIM_CC4S;
            tmp_ccmrx &= ~(m_code << 8);
            tmp_ccmrx &= ~s_code;
            tmp_ccmrx |= uint16_t(raw_code) << 8;
            instance->CHCTLR2 = tmp_ccmrx;
            break;
        }
    }
}

void TimerOut::enable_output(const Enable en) {
    if (en.to_bool()) {
        instance->CCER |= (1 << ((uint8_t)idx_ * 2));
    } else {
        instance->CCER &= ~(1 << ((uint8_t)idx_ * 2));
    }
}

void TimerOC::enable_cvr_sync(const Enable en) {
    using enum ChannelIndex;
    const auto preload = en.to_bool() ? TIM_OCPreload_Enable : TIM_OCPreload_Disable;
    switch (idx_) {
        case CH1:
            TIM_OC1PreloadConfig(instance, preload);
            break;
        case CH2:
            TIM_OC2PreloadConfig(instance, preload);
            break;
        case CH3:
            TIM_OC3PreloadConfig(instance, preload);
            break;
        case CH4:
            TIM_OC4PreloadConfig(instance, preload);
            break;
        default:
            sys::abort();
            break;
    }
}

Gpio &TimerOC::io() { return internal::get_pin(instance, idx_); }
Gpio &TimerOCN::io() { return internal::get_pin(instance, idx_); }
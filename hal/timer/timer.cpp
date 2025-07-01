#include "timer.hpp"

#include <sys/types.h>

#include <bit>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <tuple>
#include <utility>

#include "core/clock/clock.hpp"
#include "core/system.hpp"
#include "hal/nvic/nvic.hpp"
#include "hal/timer/timer_utils.hpp"
#include "sdk/Core/core_riscv.h"
#include "sdk/Peripheral/inc/ch32v30x.h"
#include "sdk/Peripheral/inc/ch32v30x_gpio.h"
#include "sdk/Peripheral/inc/ch32v30x_tim.h"

#define TIM1_RM_A8_A9_A10_A11__B13_B14_B15 0
#define TIM1_RM_A8_A9_A10_A11__A7_B0_B1 1
#define TIM1_RM_E9_E11_E13_E14__E8_E10_E12 3

#define TIM2_RM_A0_A1_A2_A3 0
#define TIM2_RM_A15_B3_A2_A3 1
#define TIM2_RM_A0_A1_B10_B11 2
#define TIM2_RM_A15_B3_B10_B11 3

#define TIM3_RM_A6_A7_B0_B1 0
#define TIM3_RM_B4_B5_B0_B1 2

#define TIM4_RM_B6_B7_B8_B9 0
#define TIM4_RM_D12_D13_D14_D15 1

#define TIM5_RM_A0_A1_A2_A3 0

#define TIM6_RM_A0_A1_A2_A3 0

#define TIM8_RM_C6_C7_C8_C9__A7_B0_B1 0
#define TIM8_RM_B6_B7_B8_C13__A13_A14_A15 1

#define TIM8_RM_A2_A3_A4_C4__C0_C1_C2 0
#define TIM8_RM_A2_A3_A4_C4__B0_B1_B2 1
#define TIM9_RM_D9_D11_D13_D15__D8_D10_D12 2

#define TIM10_RM_B8_B9_C3_C11__A12_A13_A14 0
#define TIM10_RM_B3_B4_B5_C14__A5_A6_A7 1
#define TIM10_RM_D1_D3_D5_D7__E3_E4_E5 2

using namespace ymd::hal;

#ifdef TIM_DEBUG
__inline void TIM_ASSERT(bool x) {
    if (!x) HALT;
}
#else
#define TIM_ASSERT(X)
#endif

static constexpr std::tuple<uint16_t, uint16_t> calc_best_arr_and_psc(const uint32_t bus_freq, const uint32_t tim_freq,
                                                                      std::pair<uint16_t, uint16_t> arr_range) {
    const auto [min_arr, max_arr] = arr_range;
    const unsigned int target_div = bus_freq / tim_freq;

    auto calc_psc_from_arr = [target_div](const uint16_t arr) -> uint16_t { return CLAMP(int(target_div) / (int(arr) + 1) - 1, 0, 0xFFFF); };

    [[mabe_unused]]
    auto calc_arr_from_psc = [target_div](const uint16_t psc) -> uint16_t { return CLAMP(int(target_div) / (int(psc) + 1) - 1, 0, 0xffff); };

    auto calc_freqfrom_arr_and_psc = [bus_freq](const uint16_t arr, const uint16_t psc) -> uint32_t { return bus_freq / (arr + 1) / (psc + 1); };

    const auto min_psc = calc_psc_from_arr(max_arr);
    const auto max_psc = calc_psc_from_arr(min_arr);

    if (min_arr > max_arr) {
        ymd::sys::abort();
    }

    struct Best {
        uint16_t arr;
        uint16_t psc;
        uint32_t freq_err;
    };

    Best best{max_arr, min_psc, UINT32_MAX};
    for (auto arr = max_arr; arr >= min_arr; arr--) {
        const auto expect_psc = calc_psc_from_arr(arr);
        if ((expect_psc >= max_psc) or (expect_psc < min_psc)) continue;
        std::optional<uint32_t> last_freq_;
        for (auto psc = expect_psc - 2; psc < expect_psc + 2; psc++) {
            const auto freq = calc_freqfrom_arr_and_psc(arr, psc);
            if (last_freq_.has_value()) {
                if ((last_freq_.value() - tim_freq) * (freq - tim_freq) < 0) break;
            } else {
                last_freq_ = freq;
            }
            const auto freq_err = uint32_t(ABS(int(freq) - int(tim_freq)));
            if (freq_err < best.freq_err) {
                if (freq_err == 0) {
                    return {uint16_t(arr), psc};
                }
                best = {uint16_t(arr), uint16_t(arr), freq_err};
            }
        }
    }
    if (best.freq_err == UINT32_MAX) {
        ymd::sys::abort();
    }
    return {best.arr, best.psc};
}

static constexpr uint8_t calculate_deadzone_code_from_ns(const uint32_t bus_freq, const ymd::Nanoseconds ns) {
    const uint16_t scale = (ns.count() * (bus_freq / 1000000) / 1000);
    if (scale < 128) {
        return scale;
    } else if (scale < 256) {
        const uint8_t head = 0b10000000;
        const uint8_t mask = 0b00111111;
        return ((((MIN(scale, 254) >> 1) - 64) & mask) | head);
    } else if (scale < 509) {
        const uint8_t head = 0b11000000;
        const uint8_t mask = 0b00111111;
        return ((((MIN(scale, 504) >> 1) - 32) & mask) | head);
    } else if (scale < 1009) {
        const uint8_t head = 0b11100000;
        const uint8_t mask = 0b00011111;
        return ((((MIN(scale, 1008) >> 4) - 32) & mask) | head);
    } else {
        return 0xff;
    }
}

void BasicTimer::remap(const uint8_t rm) {
    switch (uint32_t(instance_)) {
#ifdef ENABLE_TIM1
        case TIM1_BASE:
            switch (rm) {
                case 0:
                    break;
                case 1:
                    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1, ENABLE);
                    break;
                case 2:
                    break;
                case 3:
                    GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);
                    break;
            }
            break;
#endif
#ifdef ENABLE_TIM2
        case TIM2_BASE:
            switch (rm) {
                case 0:
                    break;
                case 1:
                    GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);
                    break;
                case 2:
                    GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
                    break;
                case 3:
                    GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
                    break;
            }
            break;
#endif
#ifdef ENABLE_TIM3
        case TIM3_BASE:
            switch (rm) {
                case 0:
                    break;
                case 1:
                    break;
                case 2:
                    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
                    break;
                case 3:
                    GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);
                    break;
            }
#endif
#ifdef ENABLE_TIM4
        case TIM4_BASE:
            switch (rm) {
                case 0:
                    break;
                case 1:
                    GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);
                    break;
            }
            break;
#endif
#ifdef ENABLE_TIM5
        case TIM5_BASE:
            break;
#endif
#ifdef ENABLE_TIM8
        case TIM8_BASE:
            switch (rm) {
                case 0:
                    break;
                case 1:
                    GPIO_PinRemapConfig(GPIO_Remap_TIM8, ENABLE);
                    break;
            }
            break;
#endif
    }
}

uint32_t BasicTimer::get_bus_freq() { return internal::is_advanced_timer(instance_) ? sys::clock::get_apb2_freq() : sys::clock::get_apb1_freq(); }

void BasicTimer::set_psc(const uint16_t psc) { instance_->PSC = psc; }

void BasicTimer::set_arr(const uint16_t arr) { instance_->ATRLR = arr; }

void BasicTimer::set_count_mode(const TimerCOuntMode mode) {
    auto tmpcr1 = instance_->CTLR1;
    tmpcr1 &= (uint16_t)(~((uint16_t)(TIM_DIR | TIM_CMS)));
    tmpcr1 |= (uint32_t)(mode);
    tmpcr1 &= (uint16_t)(~((uint16_t)TIM_CTLR1_CKD));
    tmpcr1 |= (uint32_t)TIM_CKD_DIV1;

    instance_->CTLR1 = tmpcr1;
}

void BasicTimer::enable_arr_sync(const Enable en) {
    if (en == EN) {
        instance_->CTLR1 |= TIM_ARPE;
    } else {
        instance_->CTLR1 &= uint16_t(~TIM_ARPE);
    }
}

void BasicTimer::enable_psc_sync(const Enable en) {
    if (en == EN) {
        instance_->SWEVGR |= TIM_PSCReloadMode_Immediate;
    } else {
        instance_->SWEVGR &= uint16_t(~TIM_PSCReloadMode_Immediate);
    }
}

void BasicTimer::set_freq(const uint32_t freq) {
    const auto [arr, psc] = calc_best_arr_and_psc(get_bus_freq(), freq, {0, 65535});
    set_arr(arr);
    set_psc(psc);
}

void BasicTimer::init(const uint32_t freq, const Mode mode, const Enable en) {
    this->enable_rcc(en);
    TIM_InternalClockConfig(instance_);
    set_freq(internal::is_aligned_count_mode(mode) ? (freq * 2) : freq);
    set_count_mode(mode);
    // TODO 是否为EN,还是en
    enable_arr_sync(en);
    TIM_ClearFlag(instance_, 0x1e7f);
    TIM_ClearITPendingBit(instance_, 0x00ff);
    enable(en);
}

void BasicTimer::deinit() {
    this->enable_rcc(DISEN);
    cbs_.fill(nullptr);
}

void BasicTimer::enable(const Enable en) {
    TIM_Cmd(instance_, en.to_state());
    if (en.to_bool() and internal::is_advanced_timer(instance_)) {
        TIM_CtrlPWMOutputs(instance_, en.to_state());
    }
}

void GenericTimer::init_as_encoder(const Mode mode) {
    this->enable_rcc(EN);

    {
        TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure{
            .TIM_Prescaler = 0,
            .TIM_CounterMode = (uint16_t)mode,
            .TIM_Period = 65535,
            .TIM_ClockDivision = TIM_CKD_DIV1,
            .TIM_RepetitionCounter = 0,
        };

        TIM_TimeBaseInit(instance_, &TIM_TimeBaseStructure);
    }

    {
        TIM_ICInitTypeDef def{
            .TIM_Channel = TIM_Channel_1,
            .TIM_ICPolarity = TIM_ICPolarity_Rising,
            .TIM_ICSelection = TIM_ICSelection_DirectTI,
            .TIM_ICPrescaler = TIM_ICPSC_DIV1,
            .TIM_ICFilter = 0x0f,
        };
        TIM_ICInit(instance_, &def);

        def.TIM_Channel = TIM_Channel_2;
        TIM_ICInit(instance_, &def);
    }

    TIM_EncoderInterfaceConfig(instance_, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    TIM_Cmd(instance_, ENABLE);
}

void GenericTimer::enable_single(const Enable en) { TIM_SelectOnePulseMode(instance_, en.to_bool() ? TIM_OPMode_Repetitive : TIM_OPMode_Single); }

void GenericTimer::set_trgo_source(const TrgoSource source) { TIM_SelectOutputTrigger(instance_, uint8_t(source)); }

void AdvancedTimer::set_deadzone_ns(const Nanoseconds ns) {
    uint8_t dead = this->calculate_deadzone(ns);

    uint16_t tempreg = instance_->BDTR;
    tempreg &= 0xff00;
    tempreg |= dead;
    instance_->BDTR = tempreg;
}

uint8_t AdvancedTimer::calculate_deadzone(const Nanoseconds ns) { return calculate_deadzone_code_from_ns(this->get_bus_freq(), ns); }

void BasicTimer::enable_it(const IT &it, const NvicPriority request, const Enable en) {
    NvicPriority::enable(request, internal::it_to_irq(instance_, it), en);
    TIM_ITConfig(instance_, std::bit_cast<uint8_t>(it), en.to_state());
}

void BasicTimer::enable_cc_ctrl_sync(const Enable en) { TIM_CCPreloadControl(instance_, en.to_state()); }

#define TRY_HANDLE_IT(it)                              \
    if (itstatus & uint8_t(it)) {                      \
        invoke_callback(it);                           \
        TIM_ClearITPendingBit(instance_, uint8_t(it)); \
        return;                                        \
    }

void GenericTimer::on_cc_interrupt() {
    const auto itstatus = instance_->INTFR;
    TRY_HANDLE_IT(IT::CC1);
    TRY_HANDLE_IT(IT::CC2);
    TRY_HANDLE_IT(IT::CC3);
    TRY_HANDLE_IT(IT::CC4);
}

void GenericTimer::on_it_interrupt() {
    const auto itstatus = instance_->INTFR;
    TRY_HANDLE_IT(IT::Update);
    TRY_HANDLE_IT(IT::CC1);
    TRY_HANDLE_IT(IT::CC2);
    TRY_HANDLE_IT(IT::CC3);
    TRY_HANDLE_IT(IT::CC4);
    TRY_HANDLE_IT(IT::COM);
    TRY_HANDLE_IT(IT::Trigger);
    TRY_HANDLE_IT(IT::Break);
}

namespace ymd::hal {
#ifdef ENABLE_TIM1
AdvancedTimer timer1{TIM1};
#endif

#ifdef ENABLE_TIM2
GenericTimer timer2{TIM2};
#endif

#ifdef ENABLE_TIM3
GenericTimer timer3{TIM3};
#endif

#ifdef ENABLE_TIM4
GenericTimer timer4{TIM4};
#endif

#ifdef ENABLE_TIM5
GenericTimer timer5{TIM5};
#endif

#ifdef ENABLE_TIM6
BasicTimer timer6{TIM6};
#endif

#ifdef ENABLE_TIM7
BasicTimer timer6{TIM7};
#endif

#ifdef ENABLE_TIM8
AdvancedTimer timer8{TIM8};
#endif

#ifdef ENABLE_TIM9
AdvancedTimer timer9{TIM9};
#endif

#ifdef ENABLE_TIM10
AdvancedTimer timer10{TIM10};
#endif

}  // namespace ymd::hal
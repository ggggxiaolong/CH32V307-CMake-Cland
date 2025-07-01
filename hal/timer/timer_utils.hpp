#pragma once

#include <cstdint>
#include <functional>

#include "core/constants/enums.hpp"
#include "core/platform.hpp"
#include "core/sdk.hpp"
#include "sdk/Peripheral/inc/ch32v30x.h"
#include "sdk/Peripheral/inc/ch32v30x_tim.h"

namespace ymd::hal {

class Gpio;

enum class TimerCOuntMode : uint8_t {
    Up = TIM_CounterMode_Up,
    UpDown = TIM_CounterMode_Down,
    CenterAlignedDownTrig = TIM_CounterMode_CenterAligned1,
    CenterAlignedUpTrig = TIM_CounterMode_CenterAligned2,
    CenterAlignedDualTrig = TIM_CounterMode_CenterAligned3,
};

enum class TimerChannelIndex : uint8_t {
    CH1 = 0b000,
    CH1N = 0b001,
    CH2 = 0b010,
    CH2N = 0b011,
    CH3 = 0b100,
    CH3N = 0b101,
    CH4 = 0b110,
};

enum class TimerTrgoSource : uint8_t {
    Reset = TIM_TRGOSource_Reset,
    Enable = TIM_TRGOSource_Enable,
    Update = TIM_TRGOSource_Update,
    OC1 = TIM_TRGOSource_OC1,
    OC1R = TIM_TRGOSource_OC1Ref,
    OC2R = TIM_TRGOSource_OC2Ref,
    OC3R = TIM_TRGOSource_OC3Ref,
    OC4R = TIM_TRGOSource_OC4Ref,
};

enum class TimerIT : uint8_t {
    Update = TIM_IT_Update,
    CC1 = TIM_IT_CC1,
    CC2 = TIM_IT_CC2,
    CC3 = TIM_IT_CC3,
    CC4 = TIM_IT_CC4,
    COM = TIM_IT_COM,
    Trigger = TIM_IT_Trigger,
    Break = TIM_IT_Break,
};

enum class TimerBdtrLockLevel : uint16_t {
    Off = TIM_LOCKLevel_OFF,
    Low = TIM_LOCKLevel_1,
    Medium = TIM_LOCKLevel_2,
    High = TIM_LOCKLevel_3,
};

enum class TimerOcMode : uint8_t {
    Freeze = 0b000,
    ActiveUnlessCvr = 0b001,
    InactiveUnlessCvr = 0b010,
    ToggleWhenCvr = 0b011,
    InactiveForever = 0b100,
    ActiveForever = 0b101,
    ActiveBelowCvr = 0b110,
    InactiveAboveCvr = 0b111,
};

namespace internal {
bool is_basic_timer(const TIM_TypeDef* instance);
bool is_general_timer(const TIM_TypeDef* instance);
bool is_advanced_timer(const TIM_TypeDef* instance);
IRQn it_to_irq(const TIM_TypeDef* instance, const TimerIT it);
Gpio& get_pin(const TIM_TypeDef* instance, const TimerChannelIndex channel);

constexpr bool is_aligned_count_mode(const TimerCOuntMode mode) {
    return (mode == TimerCOuntMode::CenterAlignedDownTrig || mode == TimerCOuntMode::CenterAlignedDualTrig ||
            mode == TimerCOuntMode::CenterAlignedUpTrig);
}
}  // namespace internal
}  // namespace ymd::hal
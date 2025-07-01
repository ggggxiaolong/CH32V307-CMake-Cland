#pragma once

#include <cstdint>

#include "hal/concept/pwm_channel.hpp"
#include "hal/timer/timer_utils.hpp"
#include "sdk/Peripheral/inc/ch32v30x.h"

namespace ymd::hal {

class DmaChannel;

class TimerChannel {
   public:
    using ChannelIndex = TimerChannelIndex;

   protected:
    TIM_TypeDef* instance;
    const ChannelIndex idx_;
    static volatile uint16_t& from_channel_to_cvr(TIM_TypeDef* instance, const ChannelIndex idx);
    TimerChannel(TIM_TypeDef* instance, const ChannelIndex idx) : instance(instance), idx_(idx) {}

   public:
    TimerChannel(const TimerChannel&) = delete;
    TimerChannel(TimerChannel&&) = delete;
    TimerChannel& enableDma(const Enable en = EN);
    DmaChannel& dma() const;
};
}  // namespace ymd::hal
#pragma once

#include <cstdint>
#include <functional>

#include "core/constants/enums.hpp"
#include "core/math_defs.hpp"
#include "core/platform.hpp"
#include "core/sys_defs.hpp"
#include "dma_utils.hpp"
#include "hal/dma/dma_def.hpp"
#include "hal/nvic/nvic.hpp"
#include "hal/timer/timer_channel.hpp"
#include "sdk/Peripheral/inc/ch32v30x.h"
#include "sdk/Peripheral/inc/ch32v30x_dma.h"
using namespace ymd::hal;
extern "C" {

#ifdef ENABLE_DMA1
__interrupt void DMA1_Channel1_IRQHandler(void);
__interrupt void DMA1_Channel2_IRQHandler(void);
__interrupt void DMA1_Channel3_IRQHandler(void);
__interrupt void DMA1_Channel4_IRQHandler(void);
__interrupt void DMA1_Channel5_IRQHandler(void);
__interrupt void DMA1_Channel6_IRQHandler(void);
__interrupt void DMA1_Channel7_IRQHandler(void);

// extern ymd::hal::DmaChannel dma1Ch1;
// extern ymd::hal::DmaChannel dma1Ch2;
// extern ymd::hal::DmaChannel dma1Ch3;
// extern ymd::hal::DmaChannel dma1Ch4;
// extern ymd::hal::DmaChannel dma1Ch5;
// extern ymd::hal::DmaChannel dma1Ch6;
// extern ymd::hal::DmaChannel dma1Ch7;
#endif

#ifdef ENABLE_DMA2
__interrupt void DMA2_Channel1_IRQHandler(void);
__interrupt void DMA2_Channel2_IRQHandler(void);
__interrupt void DMA2_Channel3_IRQHandler(void);
__interrupt void DMA2_Channel4_IRQHandler(void);
__interrupt void DMA2_Channel5_IRQHandler(void);
__interrupt void DMA2_Channel6_IRQHandler(void);
__interrupt void DMA2_Channel7_IRQHandler(void);
__interrupt void DMA2_Channel7_IRQHandler(void);
__interrupt void DMA2_Channel8_IRQHandler(void);
__interrupt void DMA2_Channel9_IRQHandler(void);
__interrupt void DMA2_Channel10_IRQHandler(void);
__interrupt void DMA2_Channel11_IRQHandler(void);
#endif
}

namespace ymd::hal {

class DmaChannel {
   public:
    using Callback = std::function<void(void)>;
    using Mode = DmaMode;
    using Priority = DmaPriority;

   protected:
    void *instance;
    const uint32_t done_mask;
    const uint32_t half_mask;
    const uint8_t dma_index;
    const uint8_t channel_index;

    Callback done_cb_;
    Callback half_cb_;
    Mode mode_;

    // void enabel_rcc(const Enable en);
    // void set_peripheral_width(const size_t width);
    // void set_mem_width(const size_t width);

#ifdef ENABLE_DMA1
    friend void ::DMA1_Channel1_IRQHandler(void);
    friend void ::DMA1_Channel2_IRQHandler(void);
    friend void ::DMA1_Channel3_IRQHandler(void);
    friend void ::DMA1_Channel4_IRQHandler(void);
    friend void ::DMA1_Channel5_IRQHandler(void);
    friend void ::DMA1_Channel6_IRQHandler(void);
    friend void ::DMA1_Channel7_IRQHandler(void);
#endif

#ifdef ENABLE_DMA2
    friend void ::DMA2_Channel1_IRQHandler(void);
    friend void ::DMA2_Channel2_IRQHandler(void);
    friend void ::DMA2_Channel3_IRQHandler(void);
    friend void ::DMA2_Channel4_IRQHandler(void);
    friend void ::DMA2_Channel5_IRQHandler(void);
    friend void ::DMA2_Channel6_IRQHandler(void);
    friend void ::DMA2_Channel7_IRQHandler(void);
    friend void ::DMA2_Channel8_IRQHandler(void);
    friend void ::DMA2_Channel9_IRQHandler(void);
    friend void ::DMA2_Channel10_IRQHandler(void);
    friend void ::DMA2_Channel11_IRQHandler(void);
#endif

    static constexpr uint8_t calculate_dma_index(const void *instance) {
#ifdef ENABLE_DMA2
        return instance < DMA2_Channel1 ? 1 : 2;
#else
        return 1;
#endif
    }

    static constexpr uint8_t calculate_channel_index(const void *instance) {
        uint8_t dma_index = calculate_dma_index(instance);
        // clang-format off
        switch (dma_index) {
            #ifdef ENABLE_DMA1
            case 1:
                return (uint32_t(instance) - DMA1_Channel1_BASE) / (DMA1_Channel2_BASE - DMA1_Channel1_BASE) + 1;
            #endif
            #ifdef ENABLE_DMA1
            case 2:
                if (uint32_t(instance) < DMA2_Channel7_BASE) {
                    return (uint32_t(instance) - DMA2_Channel1_BASE) / (DMA2_Channel2_BASE - DMA2_Channel1_BASE) + 1;
                } else {
                    return (uint32_t(instance) - DMA2_Channel7_BASE) / (DMA2_Channel8_BASE - DMA2_Channel7_BASE) + 7;
                }
            #endif
            default:
                return 1;
        }
        // clang-format on
    }

    static constexpr bool dst_is_peripheral(const Mode mode) {
        switch (mode) {
            case Mode::toPeriph:
            case Mode::toPeriphCirclular:
                return true;
            default:
                return false;
        }
    }

    static constexpr uint32_t calculate_done_mask(const void *_instance) {
        uint8_t dma_index = calculate_dma_index(_instance);
        uint8_t channel_index = calculate_channel_index(_instance);
        switch (dma_index) {
#ifdef ENABLE_DMA1
            case 1:
                return (DMA1_IT_TC1 << ((CTZ(DMA1_IT_TC2) - CTZ(DMA1_IT_TC1)) * (channel_index - 1)));
#endif
#ifdef ENABLE_DMA2
            case 2:
                if (uint32_t(_instance) <= DMA2_Channel7_BASE) {
                    return ((uint32_t)(DMA2_IT_TC1 & 0xff) << ((CTZ(DMA2_IT_TC2) - CTZ(DMA2_IT_TC1)) * (channel_index - 1))) | (uint32_t)(0x10000000);
                } else {
                    return ((uint32_t)(DMA2_IT_TC8 & 0xff) << ((CTZ(DMA2_IT_TC9) - CTZ(DMA2_IT_TC8)) * (channel_index - 8))) | (uint32_t)(0x20000000);
                }
#endif
            default:
                break;
        }
        return 0;
    }

    static constexpr uint32_t calculate_half_mask(const void *_instance) {
        uint8_t dma_index = calculate_dma_index(_instance);
        uint8_t channel_index = calculate_channel_index(_instance);
        switch (dma_index) {
#ifdef ENABLE_DMA1
            case 1:
                return (DMA1_IT_HT1 << ((CTZ(DMA1_IT_HT2) - CTZ(DMA1_IT_HT1)) * (channel_index - 1)));
#endif
#ifdef ENABLE_DMA2
            case 2:
                if (uint32_t(_instance) <= DMA2_Channel7_BASE) {
                    return ((uint32_t)(DMA2_IT_HT1 & 0xff) << ((CTZ(DMA2_IT_HT2) - CTZ(DMA2_IT_HT1)) * (channel_index - 1))) | (uint32_t)(0x10000000);
                } else {
                    return ((uint32_t)(DMA2_IT_HT8 & 0xff) << ((CTZ(DMA2_IT_HT9) - CTZ(DMA2_IT_HT8)) * (channel_index - 8))) | (uint32_t)(0x20000000);
                }
#endif
            default:
                break;
        }
        return 0;
    }

    __fast_inline void on_transfer_half_interrupt() { EXECUTE(half_cb_); }
    __fast_inline void on_transfer_done_interrupt() { EXECUTE(done_cb_); }

    void enable_rcc(const Enable en);
    void set_peripheral_width(const size_t width);
    void set_mem_width(const size_t width);
    void start(void *dst, const void *src, size_t size);

    __fast_inline void set_dst_width(const size_t width) {
        if (dst_is_peripheral(mode_)) {
            set_peripheral_width(width);
        } else {
            set_mem_width(width);
        }
    }

    __fast_inline void set_src_width(const size_t width) {
        // TODO 验证判断是否写错了
        if (!dst_is_peripheral(mode_)) {
            set_peripheral_width(width);
        } else {
            set_mem_width(width);
        }
    }

   public:
    DmaChannel() = delete;
    DmaChannel(const DmaChannel &) = delete;
    DmaChannel(DmaChannel &&) = delete;

    DmaChannel(DMA_Channel_TypeDef *_instance)
        : instance(_instance),
          done_mask(calculate_done_mask(_instance)),
          half_mask(calculate_half_mask(_instance)),
          dma_index(calculate_dma_index(_instance)),
          channel_index(calculate_channel_index(_instance)) {};

    void init(const Mode mode, const Priority priority);

    void resume();

    template <typename T>
    void transfer_pph2men(auto *dst, const volatile auto *src, size_t size) {
        set_dst_width(sizeof(T) << 3);
        set_src_width(sizeof(T) << 3);

        start(reinterpret_cast<void *>(dst), reinterpret_cast<const void *>(const_cast<const T *>(reinterpret_cast<const volatile T *>(src))), size);
    }

    template <typename T>
    void transfer_mem2pph(volatile auto *dst, const auto *src, size_t size) {
        set_dst_width(sizeof(T) << 3);
        set_src_width(sizeof(T) << 3);

        start(reinterpret_cast<void *>(const_cast<T *>(reinterpret_cast<volatile T *>(dst))), reinterpret_cast<const void *>(src), size);
    }

    template <typename T>
    void transfer_mem2mem(auto *dst, const auto *src, size_t size) {
        set_dst_width(sizeof(T) << 3);
        set_src_width(sizeof(T) << 3);

        start(reinterpret_cast<void *>(const_cast<T *>(dst)), reinterpret_cast<const void *>(src), size);
    }

    size_t pending();

    void enable_it(const NvicPriority priority, const Enable enable = EN);

    void enable_done_it(const Enable enable = EN);
};
}  // namespace ymd::hal

namespace ymd::hal {

#ifdef ENABLE_DMA1
extern DmaChannel dma1Ch1;
extern DmaChannel dma1Ch2;
extern DmaChannel dma1Ch3;
extern DmaChannel dma1Ch4;
extern DmaChannel dma1Ch5;
extern DmaChannel dma1Ch6;
extern DmaChannel dma1Ch7;
#endif

#ifdef ENABLE_DMA2
extern DmaChannel dma2Ch1;
extern DmaChannel dma2Ch2;
extern DmaChannel dma2Ch3;
extern DmaChannel dma2Ch4;
extern DmaChannel dma2Ch5;
extern DmaChannel dma2Ch6;
extern DmaChannel dma2Ch7;
extern DmaChannel dma2Ch8;
extern DmaChannel dma2Ch9;
extern DmaChannel dma2Ch10;
extern DmaChannel dma2Ch11;
#endif
}  // namespace ymd::hal

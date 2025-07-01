#pragma once

#include <bit>
#include <cstddef>
#include <cstdint>

#include "core/constants/enums.hpp"
#include "core/platform.hpp"
#include "sdk/Peripheral/inc/ch32v30x.h"

namespace ymd::hal {

enum class PinSource : uint16_t {
    None,
    _0 = 1 << 0,
    _1 = 1 << 1,
    _2 = 1 << 2,
    _3 = 1 << 3,
    _4 = 1 << 4,
    _5 = 1 << 5,
    _6 = 1 << 6,
    _7 = 1 << 7,
    _8 = 1 << 8,
    _9 = 1 << 9,
    _10 = 1 << 10,
    _11 = 1 << 11,
    _12 = 1 << 12,
    _13 = 1 << 13,
    _14 = 1 << 14,
    _15 = 1 << 15,
};

class PinMask {
   private:
    uint16_t mask_;

   public:
    [[nodiscard]] constexpr PinMask() : mask_(0) {};
    [[nodiscard]] explicit constexpr PinMask(const uint16_t mask) : mask_(mask) {};
    [[nodiscard]] explicit constexpr PinMask(const PinSource source) : mask_(std::bit_cast<uint16_t>(source)) {};
    [[nodiscard]] static constexpr PinMask from_u16(const uint16_t mask) { return PinMask(mask); }
    [[nodiscard]] static constexpr PinMask from_index(const size_t index) { return PinMask(uint16_t(1 << index)); }
    [[nodiscard]] constexpr uint16_t as_u16() const { return mask_; }
    [[nodiscard]] constexpr PinSource as_source() const { return std::bit_cast<PinSource>(mask_); }
    [[nodiscard]] constexpr bool test(size_t idx) const { return mask_ & (1 << idx); }
    [[nodiscard]] constexpr PinMask modify(size_t idx, const BoolLevel level) const {
        if (level == HIGH)
            return PinMask(mask_ | (1 << idx));
        else
            return PinMask(mask_ & ~(1 << idx));
    }

    [[nodiscard]] constexpr PinMask set_bit(size_t idx) const { return PinMask(mask_ | (1 << idx)); }
    [[nodiscard]] constexpr PinMask clr_bit(size_t idx) const { return PinMask(mask_ & ~(1 << idx)); }
    [[nodiscard]] constexpr PinMask operator|(const PinMask other) const { return PinMask(mask_ | other.mask_); }
    [[nodiscard]] constexpr PinMask operator&(const PinMask other) const { return PinMask(mask_ & other.mask_); }
    [[nodiscard]] constexpr PinMask operator~() const { return PinMask(~mask_); }
    [[nodiscard]] constexpr bool operator==(const PinMask& other) const { return mask_ == other.mask_; }
    [[nodiscard]] explicit constexpr operator bool() const { return mask_ != 0; }
};

enum class PortSource : uint8_t { PA, PB, PC, PD, PE, PF };

enum class PinName : uint8_t {
#define PIN_NAME_CREATE_TEMPLATE(x, n)                                                                                                              \
    P##x##0 = n | 0b00000, P##x##1, P##x##2, P##x##3, P##x##4, P##x##5, P##x##6, P##x##7, P##x##8, P##x##9, P##x##10, P##x##11, P##x##12, P##x##13, \
    P##x##14, P##x##15, P##x##16, P##x##17, P##x##18, P##x##19, P##x##20, P##x##21, P##x##22, P##x##23, P##x##24, P##x##25, P##x##26, P##x##27,     \
    P##x##28, P##x##29, P##x##30, P##x##31

#ifndef ENABLE_GPIOA
    PIN_NAME_CREATE_TEMPLATE(A, 0),
#endif

#ifndef ENABLE_GPIOB
    PIN_NAME_CREATE_TEMPLATE(B, 0),
#endif

#ifndef ENABLE_GPIOC
    PIN_NAME_CREATE_TEMPLATE(C, 0),
#endif

#ifndef ENABLE_GPIOD
    PIN_NAME_CREATE_TEMPLATE(D, 0),
#endif

#ifndef ENABLE_GPIOE
    PIN_NAME_CREATE_TEMPLATE(E, 0),
#endif

#ifndef ENABLE_GPIOF
    PIN_NAME_CREATE_TEMPLATE(F, 0),
#endif

#ifndef ENABLE_GPIOG
    PIN_NAME_CREATE_TEMPLATE(G, 0),
#endif
};

class GpioMode {
   public:
    enum class Kind : uint8_t {
        InAnalog = 0b0000,
        InFloating = 0b0100,
        InPullUp = 0b1000,
        InPullDown = 0b1100,
        OutPushPull = 0b0011,
        OutOpenDrain = 0b0111,
        OutAfPushPull = 0b1011,
        OutAfOpenDrain = 0b1111,
    };
    using enum Kind;

   private:
    Kind kind_;

   public:
    constexpr GpioMode(const Kind kind) : kind_(kind) {}
    constexpr bool operator==(const Kind kind) const { return kind_ == kind; }
    constexpr bool operator==(const GpioMode other) const { return kind_ == other.kind_; }
    constexpr bool is_in_mode() const { return (kind_ == InAnalog || kind_ == InFloating || kind_ == InPullUp || kind_ == InPullDown); }
    constexpr bool is_out_mode() const {
        return (kind_ == OutPushPull || kind_ == OutOpenDrain || kind_ == OutAfPushPull || kind_ == OutAfOpenDrain);
    }
    constexpr bool is_outpp_mode() const { return (kind_ == OutPushPull || kind_ == OutAfPushPull); }
    constexpr bool is_outod_mode() const { return (kind_ == OutOpenDrain || kind_ == OutAfOpenDrain); }
    explicit operator uint8_t() const { return std::bit_cast<uint8_t>(kind_); }
};
}  // namespace ymd::hal
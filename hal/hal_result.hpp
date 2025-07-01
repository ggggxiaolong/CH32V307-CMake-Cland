#pragma once

#include <cstdint>
#include <optional>
#include <utility>
#include <variant>

#include "core/platform.hpp"
#include "core/stream/ostream.hpp"
#include "core/sys_defs.hpp"

namespace ymd::hal {

class HalError {
   public:
    enum class Kind : uint8_t {
        AlreadyUnderUse = 0,
        OccupiedByOther,
        WritePayloadAckTimeout,
        BusOverload,
        SelectorOutOfRange,
        NoSelector,
        PayloadNoLength,
        VerifyFailed,
        LengthOverFlow,
        InvalidArgument,
        TxLengthAndRxLengthNotEqual,
        Unspecified = 0x7f,
    };

   private:
    Kind kind_;

   public:
    // using enum Kind;

    // 枚举类型是POD（Plain Old Data） 类型，即简单的数据类型，不具备资源管理或深拷贝逻辑
    // 对于 POD 类型，即使传递的是右值引用，也无需调用 std::move 来转移资源，因为拷贝的代价非常低。
    constexpr HalError(const HalError& other) : kind_(other.kind_) { ; }
    constexpr HalError(HalError&& other) : kind_(other.kind_) {}
    constexpr HalError(const Kind& kind) : kind_(kind) {}
    constexpr HalError(Kind&& kind) : kind_(kind) {}
    constexpr HalError& operator=(const HalError& other) = default;
    constexpr HalError& operator=(HalError&& other) = default;

    constexpr bool operator==(const HalError other) const { return kind_ == other.kind_; }
    constexpr bool operator!=(const HalError other) const { return kind_ != other.kind_; }
    constexpr bool operator==(const Kind other) const { return kind_ == other; }
    constexpr bool operator!=(const Kind other) const { return kind_ != other; }
    constexpr Kind kind() const { return kind_; }
};
class HalResult {
   private:
    std::optional<HalError> error_;

   public:
    using Kind = HalError::Kind;
    // using enum Kind;
    constexpr HalResult(std::nullopt_t) : error_(std::nullopt) {};
    constexpr HalResult(const Kind& kind) : error_(HalError(kind)) {};
    constexpr HalResult(Kind&& kind) : error_(HalError(kind)) {};
    constexpr HalResult(const HalError& error) : error_(error) {};
    constexpr HalResult(HalError&& error) : error_(error) {};

    constexpr HalResult(const HalResult& other) : error_(other.error_) {};
    constexpr HalResult(HalResult&& other) : error_(other.error_) {};

    constexpr bool operator==(const HalResult& other) const { return error_ == other.error_; }
    constexpr bool operator!=(const HalResult& other) const { return error_ != other.error_; }

    // unlikely(...)：提示编译器该条件概率较低，用于优化分支预测。
    __fast_inline constexpr bool is_error() const { return unlikely(error_.has_value()); }
    __fast_inline constexpr bool is_ok() const { return likely(!error_.has_value()); }

    static consteval HalResult ok() { return {std::nullopt}; }

    template <typename Fn>
    HalResult then(Fn&& fn) {
        if (is_ok()) {
            return std::forward<Fn>(fn)();
        }
        return *this;
    }

    HalResult operator|(const HalResult rth) const {
        if (is_ok()) {
            return rth;
        }
        return *this;
    }

    const HalError unwrap_err() const { return error_.value(); }
};

}  // namespace ymd::hal

namespace ymd {
class OutputStream;
OutputStream& operator<<(OutputStream& os, const hal::HalError& error);
OutputStream& operator<<(OutputStream& os, const hal::HalResult& rest);
OutputStream& operator<<(OutputStream& os, const hal::HalError::Kind& error_kind);
}  // namespace ymd
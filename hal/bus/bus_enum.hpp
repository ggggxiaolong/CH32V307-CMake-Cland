#pragma once

#include <cstdint>

namespace ymd {

enum class CommStrategy : uint8_t { Nil, Blocking = 1, Sync = Blocking, Interrupt, Dma };

enum class CommDirection : uint8_t { RxOnly = 1, TxOnly, TxRx = RxOnly | TxOnly };

class PeripherUserId {
    using id_type = uint16_t;

   private:
    id_type id_;
    PeripherUserId(const PeripherUserId &) = default;
    PeripherUserId(PeripherUserId &&) = default;

   public:
    PeripherUserId(const id_type id) : id_(id) {};
    PeripherUserId clone() const { return *this; }
    void emplace(const PeripherUserId &other) { id_ = other.id_; }
    void emplace(PeripherUserId &&other) { id_ = other.id_; }
};

using BusUserId = PeripherUserId;

// 用途说明： 为仲裁器提供当前外设的用法信息
template <typename TEnum>
class PeripherPurpose {
   private:
    // 用法说明符: 为仲裁器提供当前啊外设的用法信息
    // 例如 当前I2C外设可以被[PMBUS]作为[写用法]征用
    uint8_t usage_ : 4;  // 保存用途

    // 为仲裁器提供房前外设的转换信息
    //  例如 当前的Uart设备当作Rs232 设备使用
    uint8_t transform_ : 4;  // 保存转换
};

enum class I2cUsage : uint8_t { Read, Write };
enum class PmbusUsage : uint8_t { Write, Read };
}  // namespace ymd
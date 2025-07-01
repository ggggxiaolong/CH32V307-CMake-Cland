#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "core/constants/enums.hpp"
#include "core/sys_defs.hpp"
#include "gpio_intf.hpp"
#include "gpio_port_intf.hpp"
#include "hal/gpio/gpio_utils.hpp"

namespace ymd::hal {

template <size_t N>
class VGpioPortIntf : public GpioPortIntf {
   public:
    bool is_index_valid(const size_t index) { return index < N; }

   public:
    constexpr size_t size() const { return N; }
    void set_by_mask(const PinMask mask) override {
        const auto raw = read_mask();
        write_by_mask(raw | mask);
    }

    void clr_by_mask(const PinMask mask) override {
        const auto raw = read_mask();
        write_by_mask(raw & ~mask);
    }
};

template <size_t N>
class VGpioPort final : public VGpioPortIntf<N> {
   protected:
    using E = hal::GpioIntf;
    std::array<E *, N> p_pins_ = {nullptr};

    void write_by_mask(const PinMask mask) override {
        for (size_t i = 0; i < N; i++) {
            p_pins_[i]->write(BoolLevel::from(mask.test(i)));
        }
    }

    PinMask read_mask() const override {
        uint16_t mask = 0;
        for (size_t i = 0; i < N; i++) {
            mask |= uint16_t(p_pins_[i]->read().to_bool() << i);
        }
        return PinMask(mask);
    }

   public:
    VGpioPort() {};
    VGpioPort(const VGpioPort<N> &other) { p_pins_ = other.p_pins_; }
    VGpioPort(VGpioPort<N> &&other) { p_pins_ = std::move(other.p_pins_); }

    void bing_pin(hal::GpioIntf &gpio, const size_t index) {
        if (index >= N)
            while (true);
        p_pins_[size_t(index)] = &gpio;
    }

    bool is_index_valid(const size_t index) { return likely(index >= 0) and likely(index < N) and likely(p_pins_[index] != nullptr); }

    bool is_index_empty(const size_t index) { return likely(index >= 0) and likely(index < N) and likely(p_pins_[index] == nullptr); }

    void write_by_index(const size_t index, const BoolLevel level) override {
        if (!is_index_valid(index)) return;
        p_pins_[index]->write(level);
    }

    BoolLevel read_by_index(const size_t index) override {
        if (!is_index_valid(index)) return LOW;
        return p_pins_[index]->read();
    }

    void set_by_mask(const PinMask mask) override {
        for (auto i = 0; i < N; i++) {
            if (mask.test(i)) {
                p_pins_[i]->set();
            }
        }
    }

    void clr_by_mask(const PinMask mask) override {
        for (auto i = 0; i < N; i++) {
            if (mask.test(i)) {
                p_pins_[i]->clr();
            }
        }
    }

    E *begin() { return p_pins_.front(); }

    E *end() { return p_pins_.back(); }

    E &operator[](const size_t index) {
        if (is_index_valid(index)) {
            return *p_pins_[index];
        }
        while (true);
    }

    void set_mode(const size_t index, const GpioMode mode) override {
        if (!is_index_valid(index)) return;
        p_pins_[index]->set_mode(mode);
    }
};
}  // namespace ymd::hal
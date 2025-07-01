#pragma once

#include <cstdint>

#include "chipname.hpp"
#include "core/platform.hpp"
#include "core/sdk.hpp"

namespace ymd::sys {
void preinit();

__attribute__((noreturn)) void abort();
void trip();

__attribute__((noreturn)) void reset();

void dump();
void jumpto(const uint32_t addr);

namespace chip {
uint64_t get_chip_id();
uint32_t get_chip_id_crc();
uint32_t get_flash_size();
uint64_t get_mac_address();

uint32_t get_dev_id();
uint32_t get_rev_id();
}  // namespace chip

namespace exception {
void disable_interrupt();
void enable_interrupt();
bool is_interrupt_pending();
bool is_interrupt_acting();
uint8_t get_interrupt_depth();
}  // namespace exception

namespace clock {
uint32_t get_system_freq();
uint32_t get_apb1_freq();
void set_apb1_freq(uint32_t freq);
uint32_t get_apb2_freq();
void set_apb2_freq(uint32_t freq);
uint32_t get_ahb_freq();
void set_ahb_freq(uint32_t freq);

void set_apb1_div(const uint8_t div);
void set_apb2_div(const uint8_t div);
void set_ahb_div(const uint8_t div);
}  // namespace clock

}  // namespace ymd::sys
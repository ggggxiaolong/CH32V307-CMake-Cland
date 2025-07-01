#include "nvic.hpp"

#include "core/constants/enums.hpp"
#include "sdk/Core/core_riscv.h"
using namespace ymd::hal;

NvicPriority::NvicPriority(const uint8_t pre, const uint8_t sub) : _pre(pre & 0b1), _sub(sub & 0b111) {}

void NvicPriority::enable(const NvicPriority& request, const IRQn _irq, const Enable en) {
    NVIC_SetPriority(_irq, (request._pre << 7) | (request._sub << 4));
    if (en == EN) {
        NVIC_EnableIRQ(_irq);
    } else {
        NVIC_DisableIRQ(_irq);
    }
}

void NvicPriority::enable(const IRQn _irq, const Enable en) const { enable(*this, _irq, en); }

void NvicRequest::enable(const NvicRequest& request, const Enable en) { NvicPriority::enable(request._priority, request._irq, en); }

void NvicRequest::enable(const Enable en) const { enable(*this, en); }
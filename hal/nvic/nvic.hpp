#pragma once
#include <cstdint>

#include "core/constants/enums.hpp"
#include "core/platform.hpp"
#include "core/sdk.hpp"
#include "sdk/Core/core_riscv.h"

#if defined(WCH) && defined(RISCV)
#define SUPPORT_VTF
#endif

namespace ymd::hal {

struct NvicPriority;
class NvicRequest;

struct NvicPriority {
   protected:
    const uint8_t _pre;
    const uint8_t _sub;

   public:
    NvicPriority(const uint8_t pre, const uint8_t sub);
    static void enable(const NvicPriority& request, const IRQn _irq, const Enable enable = EN);
    void enable(const IRQn _irq, const Enable enable = EN) const;
};

class NvicRequest {
   protected:
    const NvicPriority _priority;
    const IRQn _irq;

   public:
    NvicRequest(const uint8_t pre, const uint8_t sub, const IRQn irq) : _priority(pre, sub), _irq(irq) {}
    NvicRequest(const NvicPriority priority, const IRQn irq) : _priority(priority), _irq(irq) {}
    void enable(const Enable en = EN) const;
    static void enable(const NvicRequest& request, const Enable en = EN);
};

#ifdef SUPPORT_VTF
class VtRequest {
   protected:
    const IRQn irq;
    const uint8_t index;
    const uint32_t fun_base;

   public:
    VtRequest(const IRQn irq, const uint8_t index, const uint32_t fun_base) : irq(irq), index(index), fun_base(fun_base) {}
    void enable(const Enable en = EN) { SetVTFIRQ(fun_base, irq, index, en == EN ? ENABLE : DISABLE); }
};
#endif

using PficRequest = NvicRequest;
}  // namespace ymd::hal
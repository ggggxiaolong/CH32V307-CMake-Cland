#include "crc.hpp"

#include "core/sdk.hpp"
#include "sdk/Core/core_riscv.h"
#include "sdk/Peripheral/inc/ch32v30x.h"
#include "sdk/Peripheral/inc/ch32v30x_crc.h"
#include "sdk/Peripheral/inc/ch32v30x_rcc.h"

using namespace ymd::hal;

Crc &crc = Crc::get_instance();

void Crc::init() {
#ifdef N32G45X
    RCC_EnableAHBPeriphClk(RCC_AHBPeriph_CRC, ENABLE);
#else
    RCC_AHBPeriphClockCmd(RCC_CRCEN, ENABLE);
#endif
}

void Crc::clear() {
#ifdef N32G45X
    CRC32_ResetCrc();
#else
    CRC_ResetDR();
#endif
}

uint32_t Crc::update(const uint32_t *begin, const uint32_t *end) {
#ifdef N32G45X
    return CRC32_CalcBufCrc(begin, end - begin);
#else
    // 修改参数类型为符合SDK定义的形式
    return CRC_CalcBlockCRC((uint32_t *)begin, (uint32_t)(end - begin));
#endif
}
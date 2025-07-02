// // #include "../Debug/debug.h"
// #include "sdk/Peripheral/inc/ch32v30x.h"
// #include "sdk/system_ch32v30x.h"

// void GPIO_INIT(void) {
//     GPIO_InitTypeDef GPIO_InitStructure;
//     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
//     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//     GPIO_Init(GPIOB, &GPIO_InitStructure);
// }

// int main() {
//     SystemCoreClockUpdate();
//     // Delay_Init();
//     // USART_Printf_Init(115200);
//     // printf("SystemClk:%ld\r\n", SystemCoreClock);
//     // printf("ChipID:%08lx\r\n", DBGMCU_GetCHIPID());
//     GPIO_INIT();
//     while (1) {
//         // printf("Hello World!\r\n");
//         // Delay_Ms(200);
//         // GPIO_WriteBit(GPIOB, GPIO_Pin_4, Bit_SET);
//         // Delay_Ms(200);
//         // GPIO_WriteBit(GPIOB, GPIO_Pin_4, Bit_RESET);
//     }
// }
#include "core/clock/clock.hpp"
#include "core/system.hpp"
#include "hal/gpio/gpio_port.hpp"

using namespace ymd;
using namespace ymd::hal;

void gpio_tb(hal::GpioIntf& gpio) {
    gpio.outpp();
    while (true) {
        gpio.set();
        clock::delay(1000ms);
        gpio.clr();
        clock::delay(1000ms);
    }
}

int main() {
    // GPIO_INIT();
    ymd::sys::preinit();
    gpio_tb(hal::portB[4]);
}

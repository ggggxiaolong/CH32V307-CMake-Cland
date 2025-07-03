#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <functional>

#include "core/buffer/ringbuf/Fifo_t.hpp"
#include "core/defines/user_defs.h"
#include "core/sdk.hpp"
#include "core/sys_defs.hpp"
#include "hal/bus/bus_base.hpp"
#include "hal/bus/bus_enum.hpp"
#include "hal/gpio/gpio.hpp"
#include "hal/hal_result.hpp"
#include "sys/_intsup.h"
#include "uart_utils.hpp"

namespace ymd::hal {

#ifndef UART_FIFO_BUF_SIZE
static constexpr size_t UART_FIFO_BUF_SIZE = 256;
#endif

#ifndef UART_DMA_BUF_SIZE
stattic constexpr size_t UART_DMA_BUF_SIZE = 64;
#endif

#ifndef UART_TX_DMA_BUF_SIZE
static constexpr size_t UART_TX_DMA_BUF_SIZE = UART_DMA_BUF_SIZE;
#endif

#ifndef UART_RX_DMA_BUF_SIZE
static constexpr size_t UART_RX_DMA_BUF_SIZE = UART_DMA_BUF_SIZE;
#endif

class Uart : public BusBase {
   public:
    using Mode = CommDirection;
    using Callback = std::function<void(void)>;
    using Parity = UartParity;

   private:
    Callback post_tx_cb_;
    Callback post_rx_cb_;

   protected:
    CommStrategy tx_strategy_;
    CommStrategy rx_strategy_;

    Fifo_t<char, UART_FIFO_BUF_SIZE> tx_fifo_;
    Fifo_t<char, UART_FIFO_BUF_SIZE> rx_fifo_;

    Uart() {}

    __fast_inline void call_post_tx_callback() { EXECUTE(post_tx_cb_); }
    __fast_inline void call_post_rx_callback() { EXECUTE(post_rx_cb_); }

   public:
    void read1(char &data);
    void readN(char *data, size_t size);

    virtual void write1(const char data) = 0;
    virtual void writeN(const char *data, size_t size) = 0;

    hal::HalResult read(uint32_t &data) {
        char _d;
        read1(_d);
        data = _d;
        return hal::HalResult::ok();
    }

    hal::HalResult write(const uint32_t data) {
        write1(char(data));
        return hal::HalResult::ok();
    }

    hal::HalResult transitive(const uint32_t data_rx, const uint32_t data_tx) {
        write(char(data_tx));
        return hal::HalResult::ok();
    }

    Uart(const Uart &) = delete;
    Uart(Uart &&) = delete;

    virtual Gpio &txio() = 0;
    virtual Gpio &rxio() = 0;

    virtual void init(const uint32_t baud, const CommStrategy rx_strategy = CommStrategy::Interrupt,
                      const CommStrategy tx_strategy = CommStrategy::Blocking) = 0;

    size_t available() const { return rx_fifo_.available(); }
    size_t pending() const { return tx_fifo_.available(); }
    size_t remain() const { return tx_fifo_.size() - tx_fifo_.available(); }

    virtual void set_tx_strategy(const CommStrategy strategy) = 0;
    virtual void set_rx_strategy(const CommStrategy strategy) = 0;
    void binding_post_tx_callback(Callback &&cb) { post_tx_cb_ = std::move(cb); }
    void binding_post_rx_callback(Callback &&cb) { post_rx_cb_ = std::move(cb); }
};

}  // namespace ymd::hal
#include "uart.hpp"

#include <span>

using namespace ymd::hal;

void Uart::read1(char &data) { data = rx_fifo_.pop(); }

void Uart::readN(char *data, size_t size) { rx_fifo_.pop(std::span(data, size)); }
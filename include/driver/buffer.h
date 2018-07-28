#ifndef UART_H
#define UART_H

#include <system/system.h>
#include "ring_buffer.h"

void buffer_init();

void uart_rx_isr(uint8_t byte);
void uart_rx_parser();

void uart_tx_isr(char data);

Buffer *get_uart_rx_buffer();
Buffer *get_uart_tx_buffer();



#endif // UART_H

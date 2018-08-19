#ifndef UART_H
#define UART_H

#include <system/system.h>
#include "buffer_ring.h"

void buffer_init();

void uart_rx_isr(uint8_t byte);
void uart_rx_parser();

void uart_tx_isr(char data);

Buffer *get_json_rx_buffer();
Buffer *get_json_tx_buffer();



#endif // UART_H

#include <driver/buffer.h>


#define UART_RX_BUFFER_LENGTH   128
#define UART_TX_BUFFER_LENGTH   8

static char UART_RX_BUFFER[UART_RX_BUFFER_LENGTH];
static char UART_TX_BUFFER[UART_TX_BUFFER_LENGTH];
static Buffer jsonRxBuf;
static Buffer jsonTxBuf;

void buffer_init()
{
    // init rxBuffer
    jsonRxBuf.buffer = UART_RX_BUFFER;
    jsonRxBuf.size = UART_RX_BUFFER_LENGTH;
    jsonRxBuf.head = jsonRxBuf.tail = jsonRxBuf.buffer;
    buffer_reset(&jsonRxBuf);

    // init txBuffer
    jsonTxBuf.buffer = UART_TX_BUFFER;
    jsonTxBuf.size = UART_TX_BUFFER_LENGTH;
    jsonTxBuf.head = jsonTxBuf.tail = jsonTxBuf.buffer;
    buffer_reset(&jsonTxBuf);
}

Buffer *get_json_rx_buffer()
{
    return &jsonRxBuf;
}

Buffer *get_json_tx_buffer()
{
    return &jsonTxBuf;
}

void uart_rx_parser()
{
    debug_message("uart_rx_parser");
}

void write_one_byte_to_uart_rx_buffer(char byte)
{
    buffer_write_one_byte(&jsonRxBuf, byte);
}

char read_one_byte_from_uart_rx_buffer()
{
    return buffer_read_one_byte(&jsonRxBuf);
}

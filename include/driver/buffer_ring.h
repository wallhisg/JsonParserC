#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <system/system.h>
#include <driver/buffer_alloc.h>

#define LF      0x0A    //  '\n' - New Line
#define CR      0x0D    //  '\r' - Enter

typedef enum {
    RING_STATUS_EMPTY = 0,
    RING_STATUS_DATA_PRESENT = 1,
    RING_STATUS_FULL = 2
} BufferStatus;

typedef struct {
    char *head;
    char *tail;
    char *buffer;
    uint8_t  size;
    BufferStatus status;
} Buffer, *BufferPtr;

void buffer_reset(Buffer *buff);

void buffer_write_one_byte(Buffer *buff, const char byte);
uint16_t buffer_write_bytes(Buffer *buff, char *bytes);

const char buffer_read_one_byte(Buffer *buff);
uint16_t buffer_read_bytes(char *result,  Buffer *buff);
char *buffer_read_bytes_with_control_letter(Buffer *buff, char ctrlLetter);

const uint16_t buffer_bytes_used(const Buffer *buff);
const uint16_t buffer_get_size(const Buffer *buff);
const BufferStatus buffer_get_status(Buffer *buff);

void buffer_consume(Buffer *buff, char ctrlChar);
void buffer_backward(Buffer *buff);
void buffer_print(Buffer *buff);

#endif // RING_BUFFER_H

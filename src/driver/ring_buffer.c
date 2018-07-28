#include <driver/ring_buffer.h>

void buffer_reset(Buffer *buff)
{
    memset(buff->buffer, 0, buff->size);
    buff->head = buff->tail = buff->buffer;
    buff->status = RING_STATUS_EMPTY;
}

void buffer_write_one_byte(Buffer *buff, const char byte)
{
//     printf("byte write %c - %d\r\n", byte, byte);
    // accept ascii code <= 127
    if(byte <= 127)
    {
        if (buff->status == RING_STATUS_FULL)
        {
            debug("RING_FULL");
            buffer_reset(buff);
            return;
        }

        *buff->tail = byte;
        buff->tail++;

        if (buff->tail == (buff->buffer + buff->size))
            buff->tail = buff->buffer;

        buff->status = RING_STATUS_DATA_PRESENT;

        if (buff->tail == buff->head)
            buff->status = RING_STATUS_FULL;
    }
}

uint16_t buffer_write_bytes(Buffer *buff, char *bytes)
{
    uint16_t numBytesWritten = 0;
    char byteWrite;
    while (numBytesWritten < buff->size)
    {
        if (buff->status == RING_STATUS_FULL)
            break;

        byteWrite = bytes[numBytesWritten++];

        if ((byteWrite == LF) || (byteWrite == CR))
        {
            buffer_write_one_byte(buff, byteWrite);
            break;
        }
        else
        {
            buffer_write_one_byte(buff, byteWrite);
        }
    }

    return numBytesWritten;
}

const char buffer_read_one_byte(Buffer *buff)
{
    char byteRead = 0;
    byteRead = *buff->head;
//    printf("byte read %d\r\n", byteRead);
    *buff->head = ' ';  // rewrite space
    buff->head++;

    if (buff->head == buff->buffer + buff->size)
        buff->head = buff->buffer;

    if (buff->head == buff->tail)
        buff->status = RING_STATUS_EMPTY;
    else
        buff->status = RING_STATUS_DATA_PRESENT;

    return byteRead;
}

uint16_t buffer_read_bytes(char *result, Buffer *buff)
{
    uint16_t numBytesRead = 0;

    while (numBytesRead < buff->size)
    {
        if (buff->status == RING_STATUS_EMPTY)
            break;
        else
        {
            result[numBytesRead++] = buffer_read_one_byte(buff);
        }
    }

    return numBytesRead;
}

const uint16_t buffer_bytes_used(const Buffer *buff)
{
    uint16_t bytesUsed = 0;

    if (buff->status == RING_STATUS_EMPTY)
        bytesUsed = 0;
    else if (buff->tail > buff->head)
        bytesUsed = (buff->tail - buff->head);
    else
        bytesUsed = (buff->size - (buff->head - buff->tail));

    return bytesUsed;
}

const uint16_t buffer_size_get(const Buffer *buff)
{
    uint16_t bytesUsed = 0;

    if(buff->status != RING_STATUS_FULL)
    {
        if(buff->head > buff->tail)
        {
            bytesUsed = buff->head - buff->tail;
        }
        else
        {
            bytesUsed = buff->size - (buff->tail - buff->head);
        }
    }

    return bytesUsed;
}

const BufferStatus get_buffer_status(Buffer *buff)
{
    return buff->status;
}

// Loop to get char out of buffer until met ctrlChar
void consume_buffer(Buffer *buff, char ctrlChar)
{
    char byte;
    while(buff->status != RING_STATUS_EMPTY)
    {
        byte = buffer_read_one_byte(buff);
        if(byte == ctrlChar)
            break;
    }
}

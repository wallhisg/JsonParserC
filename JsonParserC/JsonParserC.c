#include "JsonParserC.h"
#include <driver/buffer.h>

void WRITE_TO_JSON_TX_BUFFER(char *bytesWrite)
{
    printf("--- WRITE_TO_BUFFER ---\r\n");
    
    Buffer *buff = get_json_tx_buffer();
    uint8_t bytesWritten = strlen(bytesWrite);
    
    char byte;
    int i = 0;
    for(i = 0; i < bytesWritten; ++i)
    {
        byte = bytesWrite[i];
        
        buffer_write_one_byte(buff, byte);
    }
    printf("BUFFER STATE:%d\r\n", buff->status);
}

int main(int argc, char **argv)
{
    buffer_init();
    
    char *objWrite1 = "123456\n7";
    WRITE_TO_JSON_TX_BUFFER(objWrite1);
    Buffer *buff = get_json_tx_buffer();
    printf("BUFFER STATE:%d\r\n", buff->status);
    buffer_read_one_byte(buff);
    buffer_read_one_byte(buff);
    printf("BUFFER STATE:%d\r\n", buff->status);
    
    buffer_write_one_byte(buff,'a');
    buffer_write_one_byte(buff,'b');
    
    printf("buffer: %s\r\n", buff->buffer);
    printf("buffer_bytes_used: %d\r\n", buffer_bytes_used(buff));
    printf("BUFFER STATE:%d\r\n", buff->status);
    printf("buffer_get_size: %d\r\n", buffer_get_size(buff));
    printf("BUFFER ROLL BACK\r\n");
    buffer_backward(buff);
    printf("buffer: %s\r\n", buff->buffer);
    printf("buffer_bytes_used: %d\r\n", buffer_bytes_used(buff));
    printf("BUFFER STATE:%d\r\n", buff->status);
    printf("buffer_get_size: %d\r\n", buffer_get_size(buff));
    printf("CHAR: %c - HEAD: %p - TAIL: %p\r\n", *buff->tail, buff->head, buff->tail);
    return 0;
}

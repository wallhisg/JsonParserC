#include "json_test_func.h"
#include <driver/buffer.h>

void WRITE_TO_BUFFER(char *bytesWrite)
{
    printf("--- WRITE_TO_BUFFER ---\r\n");
    
    Buffer *rxBuff = get_json_rx_buffer();
    uint8_t bytesWritten = strlen(bytesWrite);
    
    char byte;
    int i = 0;
    for(i = 0; i < bytesWritten; ++i)
    {
        byte = bytesWrite[i];
        buffer_write_one_byte(rxBuff, byte);
    }
    printf("buffer_bytes_used: %d\r\n", buffer_bytes_used(rxBuff));    
}

void JSON_TEST_CASE()
{
    printf("--- JSON_TEST_CASE ---\r\n");

    Buffer *rxBuff = get_json_rx_buffer();
    
    // parser json object
    JsonConsume consume;
    
    while(buffer_bytes_used(rxBuff) > 0)
    {
        printf("Json input string:");
        print_buffer(rxBuff);
        json_consume_init(&consume);
        consume = get_json_type(rxBuff, &consume);
        printf("json output: \r\n");
        
        switch(consume.type)
        {
            case JSON_TYPE_STRING:
            {
                printf("Json type : %d\r\n", consume.type);
                Buffer *buff = get_json_buffer();
                printf("Json output string: %s\r\n", buff->buffer);
                break;
            }
            case JSON_TYPE_OBJECT:
            {
                printf("Json type : %d\r\n", consume.type);
                Buffer *buff = get_json_buffer();
                while(consume.counter > 3)
                {
                    consume = read_json_key(buff, &consume);
                    printf("GET KEY: %s\r\n", get_json_key());
                    consume = read_json_value(buff, &consume);
                    printf("GET VALUE: %s\r\n", get_json_value());
                }
                if(consume.counter > 0)
                    consume_buffer(buff, LF);

                break;
            }
            case JSON_TYPE_ARRAY:
            {
                printf("Json type : %d\r\n", consume.type);
                Buffer *buff = get_json_buffer();
                while(consume.counter > 3)
                {
                    consume = read_json_key(buff, &consume);
                    printf("GET KEY: %s\r\n", get_json_key());
                    consume = read_json_value(buff, &consume);
                    printf("GET VALUE: %s\r\n", get_json_value());
                }
                if(consume.counter > 0)
                    consume_buffer(buff, LF);

                break;
            }
            default:
                error("PARSER FALSE\r\n");
                break;
        }
    }
}

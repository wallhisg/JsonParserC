#include "json_test_func.h"
#include <driver/buffer.h>

extern JsonConsume tok_letter_start(const char c, JsonConsume objConsume);

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
    printf("Json input string:\r\n");
    buffer_print(rxBuff);
    
    // parser json object
    JsonConsume consume;
    json_consume_init(&consume);
    consume.nextTok = (void *)tok_letter_start;

    int i = 0;
//     for(i = 0; i < 6; ++i)
    while(buffer_bytes_used(rxBuff) > 0)
    {

//         printf("Json input string:\r\n");
//         print_buffer(rxBuff);
        
        consume = json_read_key_value(rxBuff, &consume);
        if(consume.tribool == TRIBOOL_TRUE)
        {
            printf("Json %d:\r\n", i++);
            switch(consume.type)
            {
                case JSON_TYPE_STRING:
                {
                    printf("\tJSON_TYPE: %d \tKEY: %s \t\tVALUE: %s\r\n", consume.type, json_get_key(), json_get_value());
                    break;
                }
                case JSON_TYPE_OBJECT:
                {
                    printf("\tJSON_TYPE: %d \tKEY: %s \t\tVALUE: %s\r\n", consume.type, json_get_key(), json_get_value());
                    break;
                }
                case JSON_TYPE_ARRAY:
                {
                    printf("\tJSON_TYPE: %d \tKEY: %s \t\tVALUE: %s\r\n", consume.type, json_get_key(), json_get_value());
                    break;
                }
                default:
                    break;
            }

        }
    }

}

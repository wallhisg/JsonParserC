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
    print_buffer(rxBuff);
    
    // parser json object
    JsonConsume consume;
    json_consume_init(&consume);
    consume.nextTok = (void *)tok_letter_start;

    int i = 0;
    for(i = 0; i < 6; ++i)
    {

        consume = json_read_key(rxBuff, &consume);
        if(consume.tribool == TRIBOOL_TRUE)
        {
            printf("Json %d:\r\n", i);
            switch(consume.type)
            {
                case JSON_TYPE_STRING:
                {
                    printf("JSON_TYPE: %d\r\n", consume.type);
                    printf("\tKEY: %s\r\n", json_get_key());
                    printf("\tVALUE: %s\r\n", json_get_value());
                    break;
                }
                case JSON_TYPE_OBJECT:
                {
                    printf("JSON_TYPE: %d\r\n", consume.type);
                    printf("\tKEY: %s\r\n", json_get_key());
                    printf("\tVALUE: %s\r\n", json_get_value());
                    break;
                }
                case JSON_TYPE_ARRAY:
                {
                    printf("JSON_TYPE: %d\r\n", consume.type);
                    printf("\tKEY: %s\r\n", json_get_key());
                    printf("\tVALUE: %s\r\n", json_get_value());
                    break;
                }
                default:
                    break;
            }
        }
    }
//    consume.nextTok = tok_letter_start;
//    consume = json_read_key(rxBuff, &consume);
//    printf("\tJSON_TYPE: %d\r\n", consume.type);
//    printf("\tKEY: %s\r\n", json_get_key());
//    printf("\tVALUE: %s\r\n", json_get_value());

//    consume = json_read_value(rxBuff, &consume);

//    consume = get_json_type(rxBuff, &consume);
//     Buffer *buff = get_json_buffer();
//     
//     consume.nextTok = (void*)tok_letter_start;
//     consume = read_json_key_value(buff, &consume);
//     printf("JSON TYPE: %d\r\n", consume.type);
//     printf("Key: %s\r\n", get_json_key());
//     printf("Value: %s\r\n", get_json_value());
// 
//     consume = read_json_key_value(buff, &consume);
//     printf("Key: %s\r\n", get_json_key());
//     printf("Value: %s\r\n", get_json_value());    

    
    
}

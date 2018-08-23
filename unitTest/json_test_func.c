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
    
    JsonValues *jsonVal = json_parser(rxBuff);

    if(jsonVal != NULL)
    {
        display_json_values(jsonVal);
        remove_json_values(jsonVal);
    }
    else
        error("Json parser error");

    jsonVal = json_parser(rxBuff);

    if(jsonVal != NULL)
    {
        display_json_values(jsonVal);
        remove_json_values(jsonVal);
    }
    else
        error("Json parser error");


}

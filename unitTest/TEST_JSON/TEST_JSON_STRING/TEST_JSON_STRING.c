#include <system/system.h>
#include <driver/buffer.h>
#include <json/json.h>
#include <json/json_parser.h>

extern Buffer jsonRxBuf;

#define MAX_NO_OF_BUFFER    20
static char buffer[MAX_NO_OF_BUFFER];
volatile uint16_t bytesWritten = 0;

void TEST_JSON_STRING()
{
    printf("--- TEST_JSON_STRING ---\r\n");
    memset(buffer, 0, MAX_NO_OF_BUFFER);
    Buffer *rxBuf = get_uart_rx_buffer();
    
    // init value to write
    char *bytesWrite = "\"abcdef123\"\r\n";
    uint8_t bytesWritten = strlen(bytesWrite);

    memcpy(buffer, bytesWrite, bytesWritten);
    printf("json string input in HEX: \r\n");
    int i = 0;
    for(i = 0; i < bytesWritten; i++)
    {
        printf(" %x", bytesWrite[i]);
    }    
    
    printf("\r\n");
    printf("json string input in CHAR: %s\r\n", bytesWrite);

    char byte = 0;    
    for(i = 0; i < bytesWritten; i++)
    {
        byte = buffer[i];
        buffer_write_one_byte(rxBuf, byte);
    }    
    
    printf("json string output: \r\n");
    // parser json string
    JsonType jsonType = get_json_type(rxBuf);
    
        if(jsonType == JSON_TYPE_STRING)
        {
            // print out json_parser result
            printf("Json type: %d\r\n", jsonType);
            Buffer *buff = get_json_buffer();
            printf("json string: %s\r\n", buff->buffer);
        }
        else
        {
            error("PARSER FALSE\r\n");
        }
}   

int main()
{
    system_init();
    json_init();
    
    TEST_JSON_STRING();
    
    
    return 0;
}

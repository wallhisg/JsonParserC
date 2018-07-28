#include <driver/ring_buffer.h>
#include <json/json.h>
#include <json/json_parser.h>
#include <json/json_parser_object.h>


#define MAX_NO_OF_BUFFER    64
static char buffer[MAX_NO_OF_BUFFER];
volatile uint16_t bytesWritten = 0;

void TEST_JSON_OBJECT(char *bytesWrite)
{
    printf("--- TEST_JSON_OBJECT ---\r\n");
    memset(buffer, 0, MAX_NO_OF_BUFFER);
    Buffer *rxBuf = get_uart_rx_buffer();
    
    uint8_t bytesWritten = strlen(bytesWrite);

    memcpy(buffer, bytesWrite, bytesWritten);
    
    printf("json string input in CHAR: %s\r\n", bytesWrite);

    int i = 0;
    char byte = 0;    
    for(i = 0; i < bytesWritten; i++)
    {
        byte = buffer[i];
        buffer_write_one_byte(rxBuf, byte);
    }    
    
    printf("json string output: \r\n");
    
    // parser json array
    JsonType jsonType = get_json_type(rxBuf);

    if(jsonType == JSON_TYPE_OBJECT)
    {
            printf("Json type : %d\r\n", jsonType);
        Buffer *buff = get_json_buffer();
        if(json_parser_object(buff))
        {
            int counter = 0;
            counter = get_no_of_json_object();
            printf("get_no_of_json_object: %d\r\n", counter);

            if(counter)
            {
                JsonValuePtr keyValue;
                for(i = 0; i < counter; i++)
                {
                    keyValue = get_json_object();
                    printf("Key: %s\r\n", keyValue->key);
                    printf("Value: %s\r\n", keyValue->value);
                    clear_json_object(keyValue);
                }
            }
            counter = get_no_of_json_object();
            printf("get_no_of_json_object: %d\r\n", counter);
        }
        else
        {
            error("PARSER FALSE\r\n");
        }
    }    
}

int main()
{
    system_init();
    json_init();
    // init value to write
//     char *obj1 = "{\"a\":\"1\",\"b\":\"2\"}\r\n";
    char *obj1 = "{\"name\":\"Jack\",\"age\":\"27\"}\r\n";
    TEST_JSON_OBJECT(obj1);

//    system_init();
   json_init();
    char *obj2 = "{\"a\":{\"X\":\"1\",\"Y\":\"1\"},\"b\":{\"Z\":\"1\"}}\r\n";
    TEST_JSON_OBJECT(obj2);

//    system_init();
   json_init();
    char *obj3 = "{\"a\":{\"X\":\"1\",\"Y\":\"1\"},\"b\":\"1\"}\r\n";
    TEST_JSON_OBJECT(obj3);
    
    return 0;
}

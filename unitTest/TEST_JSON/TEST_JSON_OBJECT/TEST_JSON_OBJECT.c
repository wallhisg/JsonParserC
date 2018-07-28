#include <system/error.h>
#include <json/json.h>
#include <driver/buffer.h>
#include <json/json_parser.h>
#include <json/json_parser_object.h>

#define MAX_NO_OF_BUFFER    64
static char buffer[MAX_NO_OF_BUFFER];
volatile uint16_t bytesWritten = 0;

void TEST_JSON_OBJECT(char *bytesWrite)
{
    printf("--- TEST_JSON_OBJECT ---\r\n");
    memset(buffer, 0, MAX_NO_OF_BUFFER);
    Buffer *rxBuf = get_json_rx_buffer();
    
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
    
    // parser json object
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
                JsonValue *keyValue;
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
            error("PARSER OBJECT FALSE\r\n");
        }
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
//    // TEST FAILE
//    // init values to write
   char *obj10 = "{\"a\":\"1\",\"bc\"1:\"2\"}\r\n";
   TEST_JSON_OBJECT(obj10);

   char *obj20 = "{\"a\":1{\"X\":\"1\",\"Y\":\"1\"},\"bV\":{\"Z\":\"1\"}}\r\n";
   TEST_JSON_OBJECT(obj20);

   char *obj30 = "{\"a\":{\"X\":\"1\",\"Y\":\"1\"},1\"bV\":\"12\"}\r\n";
   TEST_JSON_OBJECT(obj30);

    // TEST TRUE
    // init values to write
    char *obj1 = "{\"aA\":\"1\",\"b\":\"2\"}\r\n";
    TEST_JSON_OBJECT(obj1);

    char *obj2 = "{\"A\":{\"XX\":\"12\",\"YY\":\"12\"},\"V\":{\"Z\":\"12\"}}\r\n";
    TEST_JSON_OBJECT(obj2);

//    char *obj3 = "{\"aB\":{\"XX\":\"1\",\"YY\":\"1\"},\"bV\":\"1\"}\r\n";
    char *obj3 = "{\"d\":\"1\",\"b\":\"2\"}\r\n";
    TEST_JSON_OBJECT(obj3);

    
    return 0;
}

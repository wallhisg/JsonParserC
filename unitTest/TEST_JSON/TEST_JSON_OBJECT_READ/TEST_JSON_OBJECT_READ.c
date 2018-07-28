#include <driver/ring_buffer.h>
#include <json/json.h>
#include <json/json_parser.h>
#include <json/json_parser_object.h>
#include <json/json_reader.h>

extern JsonConsume tok_obj_letter_start(const char c, JsonConsume objConsume);
extern JsonConsume tok_obj_dq_mark(const char c, JsonConsume objConsume);
extern JsonConsume tok_obj_colon(const char c, JsonConsume objConsume);
extern JsonConsume tok_obj_comma(const char c, JsonConsume objConsume);
extern JsonConsume tok_obj_letter(const char c, JsonConsume objConsume);
extern JsonConsume tok_obj_l_curly(const char c, JsonConsume objConsume);
extern JsonConsume tok_obj_r_curly(const char c, JsonConsume objConsume);
extern JsonConsume tok_obj_l_bracket(const char c, JsonConsume objConsume);
extern JsonConsume tok_obj_r_bracket(const char c, JsonConsume objConsume);

#define MAX_NO_OF_BUFFER    64
static char buffer[MAX_NO_OF_BUFFER];
volatile uint16_t bytesWritten = 0;

void TEST_JSON_OBJECT_READ(char *bytesWrite)
{
    printf("--- TEST_JSON_OBJECT_READ ---\r\n");
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
    
    // parser json object
    JsonType jsonType = get_json_type(rxBuf);
    
    if(jsonType == JSON_TYPE_OBJECT)
    {
        printf("Json type : %d\r\n", jsonType);
        Buffer *buff = get_json_buffer();
//         if(json_object_read_description(buff))
        {
            read_key_value(buff);
//             json_object_read_description(buff);
//             json_object_read_description_value(buff);
//             json_object_read_description(buff);
//             json_object_read_description_value(buff);
//             json_object_read_description(buff);
//             json_object_read_description_value(buff);
/*            json_object_read_description(buff);
            json_object_read_description_value(buff); */           

        }
//         else
        {
//             error("PARSER OBJECT FALSE\r\n");
        }
    }   
    else if(jsonType == JSON_TYPE_ARRAY)
    {
                printf("Json type : %d\r\n", jsonType);
        Buffer *buff = get_json_buffer();
//         if(json_object_read_description(buff))
            read_key_value(buff);
    }
    else
        error("PARSER FALSE\r\n");
}

int main()
{
    system_init();
    json_init();


    // init values to write
//     char *obj1 = "{\"d\":\"1\",\"b\":\"2\"}\r\n";
//     TEST_JSON_OBJECT_READ(obj1);

//     char *obj2 = "{\"A\":{\"XX\":\"12\",\"YY\":\"12\"},\"V\":{\"Z\":\"12\"}}\r\n";
//     TEST_JSON_OBJECT_READ(obj2);
    
    char *bytesWrite2 = "{\"a\":[\"X\",\"Y\",\"Z\"],\"b\":\"1\"}\r\n";
    TEST_JSON_OBJECT_READ(bytesWrite2);
// 
//     char *obj3 = "{\"a\":{\"X\":\"1\",\"Y\":\"1\"},\"b\":\"1\"}\r\n";
//     TEST_JSON_OBJECT_READ(obj3);

    
    return 0;
}

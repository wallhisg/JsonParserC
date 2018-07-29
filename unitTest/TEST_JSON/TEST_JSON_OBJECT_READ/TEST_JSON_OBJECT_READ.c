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

void TEST_JSON_OBJECT_READ()
{
    printf("--- TEST_JSON_OBJECT_READ ---\r\n");

    Buffer *rxBuff = get_json_rx_buffer();


    
    // parser json object
    JsonConsume consume;
    
    
    while(buffer_bytes_used(rxBuff) > 0)
    {
        printf("Json input string: \r\n%s\r\n", rxBuff->buffer);
        json_consume_init(&consume);
        consume = get_json_type(rxBuff, &consume);
        printf("json output: \r\n");
        printf("consume.couter: %d\r\n", consume.counter);
        
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
                printf("consume.couter: %d\r\n", consume.counter);
                while(consume.counter > 3)
                {
                    consume = read_json_key(buff, &consume);
                    consume = read_json_value(buff, &consume);
                    printf("consume.couter: %d\r\n", consume.counter);
                    
                }
                if(consume.counter > 0)
                    consume_buffer(buff, LF);

                break;
            }
            case JSON_TYPE_ARRAY:
            {
                printf("Json type : %d\r\n", consume.type);
                Buffer *buff = get_json_buffer();
                printf("consume.couter: %d\r\n", consume.counter);
                while(consume.counter > 3)
                {
                    consume = read_json_key(buff, &consume);
                    consume = read_json_value(buff, &consume);
                    printf("consume.couter: %d\r\n", consume.counter);
                    
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

void TEST_JSON_OBJECT_READ_FALSE()
{
    system_init();
    json_init();
    // init values to write
    
    // String
    char *strWrite = "\" abcdef123\"\r\n";
    WRITE_TO_BUFFER(strWrite);
    
    // Object
     char *objWrite = "{\"A\":{\"X\":\"2\",1\"Y\":\"2\"},\"V\":{\"Z\":\"12\"}}\r\n";
     WRITE_TO_BUFFER(objWrite);

     // Array
     char *arrayWrite = "{\"a\":d[\"X\",\"Y\",\"Z\"]}\r\n";
    WRITE_TO_BUFFER(arrayWrite);

    TEST_JSON_OBJECT_READ();    
}

void TEST_JSON_OBJECT_READ_TRUE()
{
    system_init();
    json_init();
    // init values to write
    
    // String
    char *strWrite = "\"abcdef123\"\r\n";
//     WRITE_TO_BUFFER(strWrite);
    
    // Object
     char *objWrite = "{\"A\":{\"X\":\"2\",\"Y\":\"2\"},\"V\":{\"Z\":\"12\"}}\n";
     WRITE_TO_BUFFER(objWrite);

     // Array
     char *arrayWrite = "{\"a\":[\"X\",\"Y\",\"Z\"]}\n";
    WRITE_TO_BUFFER(arrayWrite);

    TEST_JSON_OBJECT_READ();
}
int main()
{
//     TEST_JSON_OBJECT_READ_FALSE();
    TEST_JSON_OBJECT_READ_TRUE();
    
    return 0;
}

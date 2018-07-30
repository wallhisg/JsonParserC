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
                    consume = read_json_value(buff, &consume);
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

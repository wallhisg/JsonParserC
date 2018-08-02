#include <driver/ring_buffer.h>
#include <json/json.h>
#include <json/json_parser.h>
#include <json/json_reader.h>
#include <json/json_writer.h>
#include "../../json_test_func.h"


void TEST_CREATE_KEY_VALUE()
{
    printf("--- TEST_CREATE_KEY_VALUE ---\r\n");
    
    JsonKeyValue kv = json_create_key_value(JSON_TYPE_STRING, "Hello", "World");
    printf("KEY: %s\r\n", kv.key);
    printf("VALUE: %s\r\n", kv.value);
}
void TEST_JSON_WRITE_KEY()
{
    system_init();
    json_init();

    printf("--- TEST_CREATE_KEY_VALUE ---\r\n");
    JsonKeyValue kv = json_create_key_value(JSON_TYPE_STRING, "Hello", "World");
    printf("KEY: %s\r\n", kv.key);
    printf("VALUE: %s\r\n", kv.value);
    
    Buffer *buff = get_json_tx_buffer();
    
    json_write_key_value(buff, kv);
    
    printf("BUFFER: %s\r\n", buff->buffer);

}

/*
void TEST_JSON_OBJECT_WRITE_TRUE()
{
    system_init();
    json_init();


    TEST_JSON_OBJECT_WRITE();
}*/

int main()
{
    system_init();
    json_init();
    TEST_CREATE_KEY_VALUE();
    TEST_JSON_WRITE_KEY();
    
    return 0;
}

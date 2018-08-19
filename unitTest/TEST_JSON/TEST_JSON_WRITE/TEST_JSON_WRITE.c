#include <driver/buffer_ring.h>
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

void TEST_JSON_WRITE_STRING()
{
    system_init();
    json_init();

    printf("--- TEST_JSON_WRITE_STRING ---\r\n");
    JsonKeyValue kv = json_create_key_value(JSON_TYPE_STRING, "Hello", "World");
    printf("KEY: %s\r\n", kv.key);
    printf("VALUE: %s\r\n", kv.value);
    
    Buffer *buff = get_json_rx_buffer();
    
    json_write_key_value(buff, kv);
    
    printf("BUFFER: %s\r\n", buff->buffer);
}

void TEST_JSON_WRITE_OBJECT()
{
    system_init();
    json_init();

    printf("--- TEST_JSON_WRITE_OBJECT ---\r\n");
    JsonKeyValue kv = json_create_key_value(JSON_TYPE_OBJECT,
                                            "name",
                                            "\"id\":\"1234\",\"method\":\"callme\"");
    printf("KEY: %s\r\n", kv.key);
    printf("VALUE: %s\r\n", kv.value);

    Buffer *buff = get_json_rx_buffer();

    json_write_key_value(buff, kv);

    printf("BUFFER: %s\r\n", buff->buffer);
}

void TEST_JSON_WRITE_ARRAY()
{
    system_init();
    json_init();

    printf("--- TEST_JSON_WRITE_ARRAY ---\r\n");
    JsonKeyValue kv = json_create_key_value(JSON_TYPE_ARRAY,
                                            "name",
                                            "\"id\",\"1234\",\"method\",\"callme\"");
    printf("KEY: %s\r\n", kv.key);
    printf("VALUE: %s\r\n", kv.value);

    Buffer *buff = get_json_rx_buffer();

    json_write_key_value(buff, kv);

    printf("BUFFER: %s\r\n", buff->buffer);
}

int main()
{
    system_init();
    json_init();
    TEST_CREATE_KEY_VALUE();
    TEST_JSON_WRITE_STRING();
    JSON_TEST_CASE();
    TEST_JSON_WRITE_OBJECT();
    JSON_TEST_CASE();
    TEST_JSON_WRITE_ARRAY();
    JSON_TEST_CASE();
    return 0;
}

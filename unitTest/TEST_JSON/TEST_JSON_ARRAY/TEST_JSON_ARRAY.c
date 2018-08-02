#include <driver/buffer.h>
#include <json/json.h>
#include <json/json_parser.h>
#include <json/json_reader.h>
#include "../../json_test_func.h"

void TEST_JSON_ARRAY_TRUE()
{
    printf("--- TEST_JSON_ARRAY_TRUE ---\r\n");

    // init values to write
    // Array
    char *arrayWrite1 = "{\"a\":[\"X\",\"Y\",\"Z\"],\"b\":[\"W\",\"X\",\"H\"]}\n";
    WRITE_TO_BUFFER(arrayWrite1);

    char *arrayWrite2 = "{\"a\":[\"X\",\"Y\",\"Z\"],\"b\":\"1\"}\r\n";
    WRITE_TO_BUFFER(arrayWrite2);

    JSON_TEST_CASE();

    Buffer *buff = get_json_buffer();
    printf("BUFFER after value");
    buffer_print(buff);
    printf("BUFFER after byte used: %d\r\n", buffer_bytes_used(buff));
}

void TEST_JSON_ARRAY_FALSE()
{
    printf("--- TEST_JSON_ARRAY_FALSE ---\r\n");
    // init values to write
    // Array
    char *arrayWrite1 = "{\"a\":[\"X\",\"Y\",\"Z\"],1\"b\":[\"W\",\"X\",\"H\"]}\n";
    WRITE_TO_BUFFER(arrayWrite1);

    char *arrayWrite2 = "{\"a\":1[\"X\",\"Y\",\"Z\"],\"b\":\"1\"}\r\n";
    WRITE_TO_BUFFER(arrayWrite2);

    JSON_TEST_CASE();

    Buffer *buff = get_json_buffer();
    printf("BUFFER after value");
    buffer_print(buff);
    printf("BUFFER after byte used: %d\r\n", buffer_bytes_used(buff));
}

int main()
{
    system_init();
    json_init();

    TEST_JSON_ARRAY_TRUE();
    TEST_JSON_ARRAY_FALSE();
    return 0;
}


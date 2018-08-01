#include <driver/buffer.h>
#include <json/json.h>
#include <json/json_parser.h>
#include <json/json_reader.h>
#include "../../json_test_func.h"

void TEST_JSON_STRING_TRUE()
{
    printf("TEST_JSON_STRING_TRUE\r\n");
    // init values to write
    // Object
    char *strWrite1 = "\"abcdef\"\n";
    WRITE_TO_BUFFER(strWrite1);
    JSON_TEST_CASE();

    Buffer *buff = get_json_buffer();
    printf("JsonBuffer: ");
    print_buffer(buff);
    printf("BUFFER after byte used: %d\r\n", buffer_bytes_used(buff));
}

void TEST_JSON_STRING_FALSE()
{
    printf("TEST_JSON_STRING_FALSE\r\n");
    // init values to write
    // Object
    char *strWrite1 = "\"123456\n\"";
    WRITE_TO_BUFFER(strWrite1);
    JSON_TEST_CASE();

    Buffer *buff = get_json_buffer();
    printf("JsonBuffer: ");
    print_buffer(buff);
    printf("BUFFER after byte used: %d\r\n", buffer_bytes_used(buff));
}
int main()
{
    printf("---------- TEST_JSON_STRING ----------\r\n");
    system_init();
    json_init();

    TEST_JSON_STRING_TRUE();
    TEST_JSON_STRING_FALSE();
    TEST_JSON_STRING_TRUE();
    return 0;
}

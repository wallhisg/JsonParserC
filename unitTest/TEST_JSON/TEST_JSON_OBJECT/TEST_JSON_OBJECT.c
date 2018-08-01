#include <driver/buffer.h>
#include <json/json.h>
#include <json/json_parser.h>
#include <json/json_reader.h>
#include "../../json_test_func.h"

void TEST_JSON_OBJECT_TRUE()
{
    printf("TEST_JSON_OBJECT_TRUE\r\n");
    // init values to write
    // Object
     char *objWrite1 = "{\"a\":\"1\",\"bc\":\"2\"}\r\n";
     WRITE_TO_BUFFER(objWrite1);

    char *objWrite2 = "{\"A\":{\"X\":\"2\",\"Y\":\"2\"},\"V\":{\"Z\":\"12\"}}\n";
    WRITE_TO_BUFFER(objWrite2);

     char *objWrite3 = "{\"A\":{\"XX\":\"12\",\"YY\":\"12\"},\"V\":{\"Z\":\"12\"}}\r\n";
     WRITE_TO_BUFFER(objWrite3);

    JSON_TEST_CASE();
    Buffer *buff = get_json_buffer();
    printf("BUFFER after value");
    print_buffer(buff);
    printf("BUFFER after byte used: %d\r\n", buffer_bytes_used(buff));
}

void TEST_JSON_OBJECT_FALSE()
{
    printf("TEST_JSON_OBJECT_FALSE\r\n");
    // init values to write
    // Object
    char *objWrite1 = "{\"a\":\"1\",\"bc\"1:\"2\"}\n";
    WRITE_TO_BUFFER(objWrite1);

    char *objWrite2 = "{\"A\":{\"X\":\"2\",\"Y\":\"2\"}a,\"V\":{\"Z\":\"12\"}}\n";
    WRITE_TO_BUFFER(objWrite2);

    char *objWrite3 = "1{\"A\":{\"XX\":\"12\",\"YY\":\"12\"},\"V\":{\"Z\":\"12\"}}\n";
    WRITE_TO_BUFFER(objWrite3);

    char *objWrite4 = "{\"A\":[\"X\":\"2\",\"Y\":\"2\"],\"V\":{\"Z\":\"12\"}}\n";
    WRITE_TO_BUFFER(objWrite4);
    JSON_TEST_CASE();
    
    Buffer *buff = get_json_buffer();
    printf("BUFFER after value");
    print_buffer(buff);
    printf("BUFFER after byte used: %d\r\n", buffer_bytes_used(buff));
}
int main()
{
    printf("---------- TEST_JSON_OBJECT ----------\r\n");
    system_init();
    json_init();

    TEST_JSON_OBJECT_TRUE();
    TEST_JSON_OBJECT_FALSE();
    return 0;
}

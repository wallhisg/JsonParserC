#include <driver/buffer.h>
#include <json/json.h>
#include <json/json_parser.h>
#include <json/json_reader.h>
#include "../../json_test_func.h"

void TEST_JSON_OBJECT_ARRAY_TRUE()
{
    printf("TEST_JSON_OBJECT_ARRAY_TRUE\r\n");
    // init values to write
    // Object

   char *objWrite2 = "{\"a\":\"1\",\"bc\":\"2\"}\n";
   WRITE_TO_BUFFER(objWrite2);

    char *objWrite3 = "{\"A\":\"123\",\"V\":{\"B\":\"45\"},\"C\":[\"d\",\"e\"]}\n";
    WRITE_TO_BUFFER(objWrite3);

    char *objWrite4 = "{\"X\":\"67\",\"Y\":{\"89\":\"12\"},\"Z\":[\"X\",\"Y\"]}\n";
    WRITE_TO_BUFFER(objWrite4);
    
    JSON_TEST_CASE();
    Buffer *buff = get_json_buffer();
    printf("BUFFER after value");
    buffer_print(buff);
    printf("BUFFER after byte used: %d\r\n", buffer_bytes_used(buff));
}

void TEST_JSON_OBJECT_ARRAY_TRUE_2()
{
    printf("TEST_JSON_OBJECT_ARRAY_TRUE\r\n");
    // init values to write
    // Object

    char *objWrite2 = "{\"name\":\"GeorgeWashington\",\"birthday\":\"February221732\",\"address\":\"MountVernonVirginiaUnitedStates\"}\n";
    WRITE_TO_BUFFER(objWrite2);

     char *objWrite3 = "{\"V\":{\"Z\":\"12\"},\"A\":[\"X\",\"Y\"]}\n";
     WRITE_TO_BUFFER(objWrite3);

    char *objWrite4 = "{\"A\":{\"X\":\"2\",\"Y\":\"2\"},\"V\":{\"Z\":\"12\"}}\n";
    WRITE_TO_BUFFER(objWrite4);
    
    JSON_TEST_CASE();
    Buffer *buff = get_json_buffer();
    printf("BUFFER after value");
    buffer_print(buff);
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

//     char *objWrite4 = "{\"A\":[\"X\":\"2\",\"Y\":\"2\"],\"V\":{\"Z\":\"12\"}}\n";
//     WRITE_TO_BUFFER(objWrite4);
    JSON_TEST_CASE();

    Buffer *buff = get_json_buffer();
    printf("BUFFER after value");
    buffer_print(buff);
    printf("BUFFER after byte used: %d\r\n", buffer_bytes_used(buff));
}
int main()
{
    printf("---------- TEST_JSON_OBJECT ----------\r\n");
    system_init();
    json_init();

    TEST_JSON_OBJECT_ARRAY_TRUE();
//     TEST_JSON_OBJECT_ARRAY_TRUE_2();
   TEST_JSON_OBJECT_FALSE();
    return 0;
}

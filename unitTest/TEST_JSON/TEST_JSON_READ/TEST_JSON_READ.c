#include <driver/buffer_ring.h>
#include <json/json.h>
#include <json/json_parser.h>
#include <json/json_reader.h>
#include "../../json_test_func.h"

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

    JSON_TEST_CASE();    
}

void TEST_JSON_OBJECT_READ_TRUE()
{
    system_init();
    json_init();
    // init values to write
    
    // String
    char *strWrite = "{\"X\":\"2\",\"Y\":\"2\"}\r\n";
    WRITE_TO_BUFFER(strWrite);
    
     // Object
      char *objWrite = "{\"A\":{\"X\":\"2\",\"Y\":\"2\"},\"V\":{\"Z\":\"12\"}}\r\n";
      WRITE_TO_BUFFER(objWrite);

      // Array
      char *arrayWrite = "{\"a\":[\"X\",\"Y\",\"Z\"]}\r\n";
     WRITE_TO_BUFFER(arrayWrite);

    JSON_TEST_CASE();
    display_heap();
}
int main()
{
    printf("\r\n"); printf("\r\n"); printf("\r\n");
//     TEST_JSON_OBJECT_READ_FALSE();
    TEST_JSON_OBJECT_READ_TRUE();
    
    return 0;
}

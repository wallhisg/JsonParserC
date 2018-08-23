#include <driver/buffer_ring.h>
#include <json/json.h>
#include <json/json_parser.h>
#include <json/json_reader.h>
#include "../../json_test_func.h"

// extern JsonList *deviceInfo;
void TEST_JSON_STRING_GET()
{
    char *jsonStr = "{\"jsonrpc\":\"2.0\",\"method\":\"subtract\",\"params\":[42,23],\"id\":1}\n";
//    json_string_get("jsonrpc",jsonStr);
}

int main()
{
    printf("\r\n"); printf("\r\n"); printf("\r\n");
    system_init();
    
//     json_list_init();
//     printf("DEVICEINFO ADDRESS: %p\r\n", deviceInfo);
//     printf("DEVICEINFO ID: %lu\r\n", deviceInfo->id);
//     destroy_json_list(deviceInfo);
    TEST_JSON_STRING_GET();
    display_heap();
    return 0;
}

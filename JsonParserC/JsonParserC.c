#include "JsonParserC.h"
#include <driver/buffer.h>
#include <json/json_writer.h>

#define MAX_NO_OF_JSON_KEY      8
typedef struct {
    char data[7];
    bool isUsed;
} JsonKey;


static JsonKey JSON_KEYS[MAX_NO_OF_JSON_KEY];

int find_free_json_key_slot()
{
    int keySlot = -1;
    int slotFound = 0;
    int i = 0;
    for(i = 0; (i < MAX_NO_OF_JSON_KEY) && (slotFound == 0); ++i)
    {
        if(!JSON_KEYS[i].isUsed)
        {
            keySlot = i;
            slotFound = 1;
        }
    }
    return keySlot;
}

int main(int argc, char **argv)
{

    printf("size of %lu\r\n", sizeof(JsonKey));
    int keySlot = find_free_json_key_slot();
    printf("KEY SLOT: %d\r\n", keySlot);

    JsonKey *key = &JSON_KEYS[keySlot];
    memset(key, 0, sizeof(JsonKey));
    memcpy(key, "123456", 6);

    key->isUsed = true;
    printf("KEY DATA: %s\r\n", key->data);
    printf("KEY : %d\r\n", key->isUsed);
    printf("KEY ADD: %p\r\n", key);
    key = NULL;
    printf("KEY ADD: %p\r\n", key);
    key = &JSON_KEYS[keySlot];
    printf("KEY ADD: %p\r\n", key);
    return 0;
}

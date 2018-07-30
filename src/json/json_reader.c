#include <json/json_reader.h>

extern JsonConsume tok_letter_start(const char c, JsonConsume objConsume);

#define JSON_KEY_LENGTH     8
#define JSON_VALUE_LENGTH   32

static char jsonKey[JSON_KEY_LENGTH];
static char jsonValue[JSON_VALUE_LENGTH];

void json_key_value_init();

JsonConsume read_json_key(Buffer *inBuff, JsonConsume *jsonConsume)
{
    printf("Buffer input: %s\r\n", inBuff->buffer);
    memset(jsonKey, 0, JSON_KEY_LENGTH);

    JsonConsume *consume = jsonConsume;
    consume->nextTok = (void*)tok_letter_start;
    consume->state = JSON_OBJECT_BEGIN;

    char byte;
    int idx = 0;
    int i = 0;
    for(i = 0; i < inBuff->size; ++i)
    {
        byte = buffer_read_one_byte(inBuff);
        consume->counter--;
        *consume = consume->nextTok(byte, consume);
        
        if ((consume->tribool == TRIBOOL_TRUE) && (consume->state == JSON_OBJECT_KEY_BEGIN))
        {
            jsonKey[idx++] = byte;
        }
        if(consume->state == JSON_OBJECT_VALUE)
            break;
    }
    return *consume;
}

JsonConsume read_json_value(Buffer *inBuff, JsonConsume *jsonConsume)
{
    printf("Buffer input: %s\r\n", inBuff->buffer);
    memset(jsonValue, 0, JSON_VALUE_LENGTH * sizeof(char));
    
    JsonConsume *consume = jsonConsume;
    
    char byte;
    int idx = 0;
    int i = 0;
    for(i = 0; i < inBuff->size; ++i)
    {
        byte = buffer_read_one_byte(inBuff);
        consume->counter--;
        *consume = jsonConsume->nextTok(byte, consume);

        if ((consume->tribool == TRIBOOL_TRUE) && (consume->state == JSON_OBJECT_VALUE_BEGIN))
        {
            jsonValue[idx++] = byte;
        }
        if(consume->state == JSON_OBJECT_VALUE_END)
            break;
    }
    
    return *consume;
}

void json_key_value_init()
{
    memset(jsonKey, 0, JSON_KEY_LENGTH * sizeof(char));
    memset(jsonValue, 0, JSON_VALUE_LENGTH * sizeof(char));
}

char * get_json_key() { return jsonKey; }
char * get_json_value() { return jsonValue; }

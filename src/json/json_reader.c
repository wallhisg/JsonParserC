#include <json/json_reader.h>

extern JsonConsume tok_obj_letter_start(const char c, JsonConsume objConsume);
extern JsonConsume tok_obj_dq_mark(const char c, JsonConsume objConsume);
extern JsonConsume tok_obj_colon(const char c, JsonConsume objConsume);
extern JsonConsume tok_obj_comma(const char c, JsonConsume objConsume);
extern JsonConsume tok_obj_letter(const char c, JsonConsume objConsume);
extern JsonConsume tok_obj_l_curly(const char c, JsonConsume objConsume);
extern JsonConsume tok_obj_r_curly(const char c, JsonConsume objConsume);
extern JsonConsume tok_obj_l_bracket(const char c, JsonConsume objConsume);
extern JsonConsume tok_obj_r_bracket(const char c, JsonConsume objConsume);

#define MAX_NO_OFF_JSON_KEY     8
#define MAX_NO_OFF_JSON_VALUE   32

JsonConsume read_json_key(Buffer *inBuff, JsonConsume *jsonConsume)
{
    printf("Buffer input: %s\r\n", inBuff->buffer);
    char key[MAX_NO_OFF_JSON_KEY];
    memset(key, 0, MAX_NO_OFF_JSON_KEY);

    JsonConsume consume;
    jsonConsume->nextTok = (void*)tok_obj_letter_start;
    jsonConsume->state = JSON_OBJECT_BEGIN;

    char byte;
    int idx = 0;
    int i = 0;
    for(i = 0; i < inBuff->size; ++i)
    {
        byte = buffer_read_one_byte(inBuff);
        consume = jsonConsume->nextTok(byte, jsonConsume);
        
        if ((consume.tribool == TRIBOOL_TRUE) && (consume.state == JSON_OBJECT_KEY_BEGIN))
        {
            key[idx++] = byte;
        }
        if(consume.state == JSON_OBJECT_VALUE)
            break;
    }
    printf("Key: %s\r\n", key);
    printf("Buffer remain: %s\r\n", inBuff->buffer);
    return consume;
}

JsonConsume read_json_value(Buffer *inBuff, JsonConsume *jsonConsume)
{
    printf("Buffer input: %s\r\n", inBuff->buffer);
    char value[MAX_NO_OFF_JSON_VALUE];
    memset(value, 0, MAX_NO_OFF_JSON_VALUE * sizeof(char));
    
    JsonConsume consume;
    
    char byte;
    int idx = 0;
    int i = 0;
    for(i = 0; i < inBuff->size; ++i)
    {
        byte = buffer_read_one_byte(inBuff);
        consume = jsonConsume->nextTok(byte, jsonConsume);
        
        if ((consume.tribool == TRIBOOL_TRUE) && (consume.state == JSON_OBJECT_VALUE_BEGIN))
        {
            value[idx++] = byte;
        }
        if(consume.state == JSON_OBJECT_BEGIN)
            break;
    }
    
    printf("value %s\r\n", value);
    printf("Buffer remain: %s\r\n", inBuff->buffer);
    return consume;
}

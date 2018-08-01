#include <json/json_reader.h>

extern JsonConsume tok_letter_start(const char c, JsonConsume objConsume);

#define JSON_KEY_LENGTH     8
#define JSON_VALUE_LENGTH   32

static char jsonKey[JSON_KEY_LENGTH];
static char jsonValue[JSON_VALUE_LENGTH];

void json_key_value_init();


JsonConsume json_read_key(Buffer *inBuff, JsonConsume *jsonConsume)
{
    json_key_value_init();
    int idxKey = 0;
    int idxVal = 0;
    char byte;
    JsonConsume consume;

    int byteUsed = buffer_bytes_used(inBuff);
    int i = 0;
    for(i = 0; i < byteUsed; ++i)
    {
        byte = buffer_read_one_byte(inBuff);
        consume = consume_char(byte, jsonConsume);
        if(consume.tribool == TRIBOOL_TRUE)
        {
            if(consume.state == JSON_STATE_NAME)
            {
                jsonKey[idxKey++] = byte;
            }
            else if((consume.state == JSON_STATE_VALUE_STR_BEGIN) ||
                    (consume.state == JSON_STATE_VALUE_OBJECT_BEGIN) ||
                    (consume.state == JSON_STATE_VALUE_ARRAY_BEGIN))
            {
                jsonValue[idxVal++] = byte;
            }
            else if(consume.state == JSON_STATE_END)
            {
                break;
            }
            else if(consume.state == JSON_END)
            {
                consume.state = JSON_STATE_BEGIN;
                consume.tribool = TRIBOOL_TRUE;
                consume.nextTok = (void *)tok_letter_start;
                break;
            }

        }
        else if(consume.tribool == TRIBOOL_FALSE)
        {
            printf("Fatal error: json string wrong at %d - %c\r\n", i, byte);
            json_key_value_init();
            consume_buffer(inBuff, LF);
            break;
        }
    }

    return consume;
}

//JsonConsume json_read_value(Buffer *inBuff, JsonConsume *jsonConsume)
//{
//    int idx = 0;
//    char byte;
//    JsonConsume consume;

//    int byteUsed = buffer_bytes_used(inBuff);
//    int i = 0;
//    for(i = 0; i < byteUsed; ++i)
//    {
//        byte = buffer_read_one_byte(inBuff);
////        consume = consume_char(byte, jsonConsume);
////        if(consume.tribool == TRIBOOL_TRUE)
////        {
////            if(consume.state == JSON_STATE_NAME)
////            {
////                jsonKey[idx++] = byte;
////            }
////            else if(consume.state == JSON_STATE_END)
////            {
////                break;
////            }
////        }
////        else if(consume.state == JSON_STATE_END)
////        {
////            break;
////        }
////        else if(consume.tribool == TRIBOOL_FALSE)
////        {
////            json_key_value_init();
////            consume_buffer(inBuff, LF);
////            break;
////        }
//    }

//    consume;
//}

void json_key_value_init()
{
    memset(jsonKey, 0, JSON_KEY_LENGTH * sizeof(char));
    memset(jsonValue, 0, JSON_VALUE_LENGTH * sizeof(char));
}

char *json_get_key() { return jsonKey; }
char *json_get_value() { return jsonValue; }
char *json_get_string() { return jsonValue; }

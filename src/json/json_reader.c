#include <json/json_reader.h>

extern JsonConsume tok_letter_start(const char c, JsonConsume objConsume);

#define JSON_KEY_READ_LENGTH     8
#define JSON_VALUE_READ_LENGTH   32

static char keyRead[JSON_KEY_READ_LENGTH];
static char valueRead[JSON_VALUE_READ_LENGTH];

void json_key_value_init();

JsonConsume json_read_key_value(Buffer *inBuff, JsonConsume *jsonConsume)
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
                keyRead[idxKey++] = byte;
            }
            else if((consume.state == JSON_STATE_VALUE_STR_BEGIN) ||
                    (consume.state == JSON_STATE_VALUE_OBJECT_BEGIN) ||
                    (consume.state == JSON_STATE_VALUE_ARRAY_BEGIN))
            {
                valueRead[idxVal++] = byte;
            }
            else if(consume.state == JSON_END)
            {
                break;
            }
        }
        else if(consume.tribool == TRIBOOL_FALSE)
        {
            printf("Fatal error: input string wrong at %d - %c\r\n", i, byte);
            json_key_value_init();
            buffer_consume(inBuff, LF);
            break;
        }
    }

    return consume;
}

void json_key_value_init()
{
    memset(keyRead, 0, JSON_KEY_READ_LENGTH * sizeof(char));
    memset(valueRead, 0, JSON_VALUE_READ_LENGTH * sizeof(char));
}

char *json_get_key() { return keyRead; }
char *json_get_value() { return valueRead; }

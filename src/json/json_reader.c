#include <json/json_reader.h>

extern JsonConsume tok_letter_start(const char c, JsonConsume objConsume);

#define JSON_KEY_READ_LENGTH     16
#define JSON_VALUE_READ_LENGTH   58

static char keyRead[JSON_KEY_READ_LENGTH];
static char valueRead[JSON_VALUE_READ_LENGTH];


void json_key_value_init();

JsonConsume json_read_frame(Buffer *inBuff)
{
    // parser json input string
    JsonConsume consume;
    json_consume_init(&consume);
    consume.nextTok = (void *)tok_letter_start;
    JsonKeyValue keyValue;

    while(buffer_bytes_used(inBuff) > 0)
    {
        memset(&keyValue, 0, sizeof(JsonKeyValue));
        consume = json_read_key_value(inBuff, &consume, &keyValue);
        if((consume.tribool == TRIBOOL_TRUE) && consume.state != JSON_END)
        {
            switch(consume.type)
            {
                case JSON_TYPE_STRING:
                {
                    printf("\tJSON_TYPE: %d \tKEY: %s \t\tVALUE: %s\r\n",
                           keyValue.type,
                           keyValue.key,
                           keyValue.value);
                    break;
                }
                case JSON_TYPE_OBJECT:
                {
                    printf("\tJSON_TYPE: %d \tKEY: %s \t\tVALUE: %s\r\n",
                           keyValue.type,
                           keyValue.key,
                           keyValue.value);
                    JsonConsume sc;
                    json_consume_init(&sc);
                    sc.nextTok = (void *)tok_letter_start;
                    char data;
                    // parser Object
                    int i = 0;
                    for(i = 0; i < strlen(keyValue.value); ++i)
                    {
                        data = keyValue.value[i];
//                        printf("DATA %c\r\n", data);
                        sc = consume_char(data, &sc);
                        if(sc.state == JSON_STATE_NAME)
                        {
                            printf("SUB KEY: %c\r\n", data);
                        }
                        else if(sc.state == JSON_STATE_VALUE_STR_BEGIN)
                        {
                            printf("SUB VALUE: %c\r\n", data);
                        }
                    }

                    break;
                }
                case JSON_TYPE_ARRAY:
                {
                    printf("\tJSON_TYPE: %d \tKEY: %s \t\tVALUE: %s\r\n",
                           keyValue.type,
                           keyValue.key,
                           keyValue.value);

                    JsonConsume sc;
                    json_consume_init(&sc);
                    sc.nextTok = (void *)tok_letter_start;
                    sc.state = JSON_STATE_NAME_BEGIN;
                    sc.type = JSON_TYPE_STRING;
                    char data;
                    // parser Object
                    int i = 0;
                    for(i = 0; i < strlen(keyValue.value); ++i)
                    {
                        data = keyValue.value[i];
//                        printf("DATA %c\r\n", data);
                        sc = consume_char(data, &sc);
                        if(sc.state == JSON_STATE_NAME)
                        {
                            printf("SUB KEY: %c\r\n", data);
                        }
                        else if(sc.state == JSON_STATE_NAME_END)
                        {
                            printf("SUB VALUE: %c\r\n", data);
                        }
                    }

                    break;
                }
                default:
                    break;
            }
            if(buffer_bytes_used(inBuff) < 3)
            {
                buffer_consume(inBuff, LF);
                break;
            }
        }
    }
    printf("buffer_bytes_used: %d\r\n", buffer_bytes_used(inBuff));
    return consume;
}

JsonConsume json_read_key_value(Buffer *inBuff, JsonConsume *jsonConsume,
                                JsonKeyValue *keyValue)
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
                keyValue->key[idxKey++] = byte;
            }
            else if((consume.state == JSON_STATE_VALUE_STR_BEGIN) ||
                    (consume.state == JSON_STATE_VALUE_OBJECT_BEGIN) ||
                    (consume.state == JSON_STATE_VALUE_ARRAY_BEGIN))
            {
                keyValue->value[idxVal++] = byte;
            }
            else if((consume.state == JSON_STATE_END) || (byte == LF))
            {
                keyValue->type = consume.type;
                break;
            }
        }
        else if(consume.tribool == TRIBOOL_FALSE)
        {
            printf("Fatal error: input string wrong at %d - %c\r\n", i, byte);
            consume.tribool = TRIBOOL_FALSE;
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

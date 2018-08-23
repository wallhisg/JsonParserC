#include <json/json_reader.h>
#include <driver/buffer_alloc.h>

#define JSON_NAME_LENGTH     8
#define JSON_VALUE_LENGTH   32

char jName[JSON_NAME_LENGTH];
char jValue[JSON_VALUE_LENGTH];

extern JsonConsume tok_letter_start(const char c, JsonConsume objConsume);

static void json_value_init();
char *json_get_header(char *jsonStr, JsonConsume *jsonConsume);
JsonValues *json_get_payload(JsonValues *head, char *jsonStr, JsonConsume *jsonConsume);
static JsonValues *json_value_wrapper(JsonType type, char *name, char *value);

JsonValues *json_parser(Buffer *buff)
{
    JsonConsume consume;
    json_consume_init(&consume);
    consume.nextTok = (void *)tok_letter_start;

    char *jsonStr = (char *)buffer_read_bytes_with_control_letter(buff, LF);
    char *jsonstrOld = jsonStr;
    printf("JSON STRING: %s\r\n", jsonStr);

    json_value_init();

    jsonStr = json_get_header(jsonStr, &consume);
    JsonValues *jsonValue = NULL;

    // read json header
    if((consume.tribool == TRIBOOL_TRUE) && (consume.type != JSON_TYPE_UNDEFINED))
    {
        jsonValue = json_value_wrapper(consume.type, jName, jValue);
    }

    // read json payload
    jsonValue = json_get_payload(jsonValue, jsonStr, &consume);
    display_heap();
    buffer_free(jsonstrOld);
    return jsonValue;
}

char *json_get_header(char *jsonStr, JsonConsume *jsonConsume)
{
    json_value_init();

    JsonConsume consume = {0};
    int idxKey = 0;
    int idxVal = 0;
    char byte;

    int len = strlen(jsonStr);
    int i = 0;
    for(i = 0; i < len; ++i)
    {
        byte = *jsonStr;
        jsonStr++;
        consume = consume_char(byte, jsonConsume);
        if(consume.tribool == TRIBOOL_TRUE)
        {
            // read name
            if(consume.state == JSON_STATE_NAME)
            {
                jName[idxKey++] = byte;
            }

            if(consume.state == JSON_STATE_VALUE_STRING)
            {
                jValue[idxVal++] = byte;
            }
            if(consume.state == JSON_STATE_VALUE_STR_END)
            {
                printf("BREAK\r\n");
                break;
            }
            if((consume.state == JSON_END) || (byte == LF))
            {
                error("Json string incorrect");
                consume.tribool = TRIBOOL_FALSE;
                break;
            }
        }
        else if(consume.tribool == TRIBOOL_FALSE)
        {
            printf("Fatal error: input string wrong at %d - %c\r\n", i, byte);
            consume.tribool = TRIBOOL_FALSE;
            break;
        }
    }

    return jsonStr;
}

JsonValues *json_get_payload(JsonValues *head, char *jsonStr, JsonConsume *jsonConsume)
{
    json_value_init();
    JsonValues *jsonValue = NULL;
    JsonConsume *consume = jsonConsume;
    int idxKey = 0;
    int idxVal = 0;
    char byte;

    int len = strlen(jsonStr);
    int i = 0;
    for(i = 0; i < len; ++i)
    {
        byte = jsonStr[i];
        *consume = consume_char(byte, jsonConsume);
        if(consume->tribool == TRIBOOL_TRUE)
        {
            // read name
            if(consume->state == JSON_STATE_NAME)
            {
                jName[idxKey++] = byte;
            }

            if((consume->state == JSON_STATE_VALUE_STRING) ||
                (consume->state == JSON_STATE_VALUE_OBJECT) ||
                (consume->state == JSON_STATE_VALUE_ARRAY))
            {
                jValue[idxVal++] = byte;
            }
            if((consume->state == JSON_STATE_VALUE_STR_END) ||
                (consume->state == JSON_STATE_VALUE_OBJECT_END) ||
                (consume->state == JSON_STATE_VALUE_ARRAY_END))
            {
                if((consume->tribool == TRIBOOL_TRUE) && (consume->type != JSON_TYPE_UNDEFINED))
                {
                    jsonValue = json_value_wrapper(consume->type, jName, jValue);
                    jsonValue = insert_json_value_by_pointer(head, jsonValue);
                }
                json_value_init();
            }
        }
        else if(consume->tribool == TRIBOOL_FALSE)
        {
            printf("Fatal error: input string wrong at %d - %c\r\n", i, byte);
            consume->tribool = TRIBOOL_FALSE;
            break;
        }
    }

    return head;
}
static JsonValues *json_value_wrapper(JsonType type, char *name, char *value)
{
    JsonValues *jsonValue = (JsonValues *)buffer_malloc(sizeof(JsonValues));
    size_t nameLen = strlen(name);
    size_t valLen = strlen(value);
    if(jsonValue != NULL)
    {
        jsonValue->name = (char *)buffer_malloc(nameLen * sizeof(char));
        jsonValue->value = (char *)buffer_malloc(valLen * sizeof(char));

        if(jsonValue->name != NULL && jsonValue->value != NULL)
        {
            memcpy(jsonValue->name, name, nameLen);
            memcpy(jsonValue->value, value, valLen);
            jsonValue->type = type;
        }
        else
        {
            buffer_free(jsonValue->name);
            buffer_free(jsonValue->value);
            jsonValue->type = JSON_TYPE_UNDEFINED;
            buffer_free(jsonValue);

            jsonValue = NULL;
        }
    }
    return jsonValue;
}

static void json_value_init()
{
    memset(jName, 0, JSON_NAME_LENGTH);
    memset(jValue, 0, JSON_VALUE_LENGTH);
}

#include <json/json_reader.h>
#include <driver/buffer_alloc.h>

#define JSON_KEY_LENGTH     8
#define JSON_VALUE_LENGTH   32

char jName[JSON_KEY_LENGTH];
char jValue[JSON_VALUE_LENGTH];

extern JsonConsume tok_letter_start(const char c, JsonConsume objConsume);
JsonObject get_json_object_value(char *jsonStr);
static void json_value_init();

JsonConsume json_read_frame(Buffer *inBuff)
{
    // parser json input string
    JsonConsume consume;
    json_consume_init(&consume);
    consume.nextTok = (void *)tok_letter_start;
    JsonValue jsonValue = {0};
    
    while(buffer_bytes_used(inBuff) > 0)
    {
        memset(&jsonValue, 0, sizeof(JsonValue));
        jsonValue = json_read_value(inBuff, &consume);
        if((consume.tribool == TRIBOOL_TRUE) && consume.state != JSON_END)
        {
            switch(consume.type)
            {
                case JSON_TYPE_STRING:
                {
                    printf("\tJSON_TYPE: %d \tKEY: %s \t\tVALUE: %s\r\n",
                           jsonValue.type,
                           jsonValue.key,
                           jsonValue.value);
                    buffer_free(jsonValue.key);
                    buffer_free(jsonValue.value);
                    break;
                }
                case JSON_TYPE_OBJECT:
                {
                    printf("\tJSON_TYPE: %d \tKEY: %s \t\tVALUE: %s\r\n",
                           jsonValue.type,
                           jsonValue.key,
                           jsonValue.value);
//                    JsonConsume sc;
//                    json_consume_init(&sc);
//                    sc.nextTok = (void *)tok_letter_start;
//                    char data;
//                    // parser Object
//                    int i = 0;
//                    for(i = 0; i < strlen(jsonValue.value); ++i)
//                    {
//                        data = jsonValue.value[i];
////                        printf("DATA %c\r\n", data);
//                        sc = consume_char(data, &sc);
//                        if(sc.state == JSON_STATE_NAME)
//                        {
//                            printf("SUB KEY: %c\r\n", data);
//                        }
//                        else if(sc.state == JSON_STATE_VALUE_STR_BEGIN)
//                        {
//                            printf("SUB VALUE: %c\r\n", data);
//                        }
//                    }

                    buffer_free(jsonValue.key);
                    buffer_free(jsonValue.value);
                    break;
                }
                case JSON_TYPE_ARRAY:
                {
                    printf("\tJSON_TYPE: %d \tKEY: %s \t\tVALUE: %s\r\n",
                           jsonValue.type,
                           jsonValue.key,
                           jsonValue.value);

//                    JsonConsume sc;
//                    json_consume_init(&sc);
//                    sc.nextTok = (void *)tok_letter_start;
//                    sc.state = JSON_STATE_NAME_BEGIN;
//                    sc.type = JSON_TYPE_STRING;
//                    char data;
//                    // parser Object
//                    int i = 0;
//                    for(i = 0; i < strlen(jsonValue.value); ++i)
//                    {
//                        data = jsonValue.value[i];
////                        printf("DATA %c\r\n", data);
//                        sc = consume_char(data, &sc);
//                        if(sc.state == JSON_STATE_NAME)
//                        {
////                            printf("SUB KEY: %c\r\n", data);
//                        }
//                        else if(sc.state == JSON_STATE_NAME_END)
//                        {
////                            printf("SUB VALUE: %c\r\n", data);
//                        }
//                    }
                    buffer_free(jsonValue.key);
                    buffer_free(jsonValue.value);
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
//    display_heap();
    return consume;
}

JsonValue json_read_value(Buffer *inBuff, JsonConsume *jsonConsume)
{
    json_value_init();

    JsonConsume consume = {0};
    int idxKey = 0;
    int idxVal = 0;
    char byte;

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
                jName[idxKey++] = byte;
            }
            else if((consume.state == JSON_STATE_VALUE_STR_BEGIN) ||
                    (consume.state == JSON_STATE_VALUE_OBJECT_BEGIN) ||
                    (consume.state == JSON_STATE_VALUE_ARRAY_BEGIN))
            {
                jValue[idxVal++] = byte;
            }
            else if((consume.state == JSON_STATE_VALUE_STR_END) ||
                    (consume.state == JSON_STATE_VALUE_OBJECT_END) ||
                    (consume.state == JSON_STATE_VALUE_ARRAY_END))
            {
                jValue[idxVal++] = byte;
            }
            else if((consume.state == JSON_STATE_VALUE_STRING) ||
                    (consume.state == JSON_STATE_VALUE_OBJECT) ||
                    (consume.state == JSON_STATE_VALUE_ARRAY))
            {
                jValue[idxVal++] = byte;
            }
            else if((consume.state == JSON_STATE_VALUE_END) || (byte == LF))
            {
                break;
            }
        }
        else if(consume.tribool == TRIBOOL_FALSE)
        {
            printf("Fatal error: input string wrong at %d - %c\r\n", i, byte);
            consume.tribool = TRIBOOL_FALSE;
            buffer_consume(inBuff, LF);
            break;
        }
    }

    JsonValue jsonValue;
    if((consume.tribool == TRIBOOL_TRUE) && (consume.type != JSON_TYPE_UNDEFINED))
    {
        size_t nameLen = strlen(jName);
        jsonValue.key = (char *)buffer_malloc(nameLen * sizeof(char));
        memcpy(jsonValue.key, jName, nameLen);
        
        size_t valLen = strlen(jValue);
        jsonValue.value = (char *)buffer_malloc(valLen * sizeof(char));
        memcpy(jsonValue.value, jValue, valLen);
        
        jsonValue.type = consume.type;
    }

    return jsonValue;
}

JsonObject get_json_object_value(char *jsonStr)
{
    JsonObject jsonObj = {0};
    JsonConsume consume;
    json_value_init();
    json_consume_init(&consume);

    consume.nextTok = (void *)tok_letter_start;

    char byte;
    size_t len = strlen(jsonStr);

    int i = 0;
    for(i = 0; i < len; ++i)
    {
        byte = jsonStr[i];
        consume = consume_char(byte, &consume);
        if(consume.tribool == TRIBOOL_TRUE)
        {
            if(consume.state == JSON_STATE_NAME)
            {
//                printf("SUB KEY: %c\r\n", byte);
            }
            else if(consume.state == JSON_STATE_VALUE_STR_BEGIN)
            {
//                printf("SUB VALUE: %c\r\n", byte);
            }
            else if((consume.state == JSON_STATE_VALUE_END) || (byte == LF))
            {
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

    return jsonObj;

}

static void json_value_init()
{
    memset(jName, 0, JSON_KEY_LENGTH);
    memset(jValue, 0, JSON_VALUE_LENGTH);
}

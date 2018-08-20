#include <json/json_writer.h>
#include <driver/buffer_alloc.h>

bool json_write_key_value(Buffer *buff, JsonValue keyValue)
{
    bool result = false;
    result = json_write_key(buff, keyValue.key);
    switch (keyValue.type)
    {
        case JSON_TYPE_STRING:
            result = json_write_value_type_string(buff, keyValue.value);
            break;
        case JSON_TYPE_OBJECT:
            result = json_write_value_type_object(buff, keyValue.value);
            break;
        case JSON_TYPE_ARRAY:
            result = json_write_value_type_array(buff, keyValue.value);
            break;
        default:
            result = false;
            break;
    }

    return result;
}

bool json_write_key(Buffer *buff, const char key[])
{
    bool result = false;
    int buffSize = buffer_get_size(buff);
    int keyLen = strlen(key);

    if(buffSize > keyLen)
    {
        buffer_write_one_byte(buff, '{');
        buffer_write_one_byte(buff, '\"');

        int i = 0;
        for(i = 0; i < keyLen; ++i)
        {
            buffer_write_one_byte(buff, key[i]);
        }

        buffer_write_one_byte(buff, '\"');
        buffer_write_one_byte(buff, ':');
        result = true;
    }
    else
        result = false;

    return result;
}

bool json_write_value_type_string(Buffer *buff, const char value[])
{
    bool result = false;
    int buffSize = buffer_get_size(buff);
    int valueLen = strlen(value);

    if(buffSize > valueLen)
    {
        buffer_write_one_byte(buff, '\"');

        int i = 0;
        for(i = 0; i < valueLen; ++i)
        {
            buffer_write_one_byte(buff, value[i]);
        }

        buffer_write_one_byte(buff, '\"');
        buffer_write_one_byte(buff, '}');
        buffer_write_one_byte(buff, LF);

        result = true;
    }
    else
        result = false;

    return result;
}

bool json_write_value_type_object(Buffer *buff, const char value[])
{
    bool result = false;
    int buffSize = buffer_get_size(buff);
    int valueLen = strlen(value);

    if(buffSize > valueLen)
    {
        buffer_write_one_byte(buff, '{');

        int len = strlen(value);
        int i = 0;
        for(i = 0; i < len; ++i)
        {
            buffer_write_one_byte(buff, value[i]);
        }

        buffer_write_one_byte(buff, '}');
        buffer_write_one_byte(buff, '}');
        buffer_write_one_byte(buff, LF);

        result = true;
    }
    else
        result = false;

    return result;
}

bool json_write_value_type_array(Buffer *buff, const char value[])
{
    bool result = false;
    int buffSize = buffer_get_size(buff);
    int valueLen = strlen(value);

    if(buffSize > valueLen)
    {
        buffer_write_one_byte(buff, '[');

        int len = strlen(value);
        int i = 0;
        for(i = 0; i < len; ++i)
        {
            buffer_write_one_byte(buff, value[i]);
        }

        buffer_write_one_byte(buff, ']');
        buffer_write_one_byte(buff, '}');
        buffer_write_one_byte(buff, LF);

        result = true;
    }
    else
        result = false;

    return result;
}

JsonValue json_create_key_value(JsonType type, char key[], char value[])
{
    JsonValue jsonValue;
    
    size_t keyLen = strlen(key);
    jsonValue.key = (char *)buffer_malloc(keyLen * sizeof(char));
    memset(&jsonValue.key, 0, keyLen);
    memcpy(&jsonValue.key, key, strlen(key));
    
    size_t valLen = strlen(value);
    jsonValue.key = (char *)buffer_malloc(valLen * sizeof(char));
    memset(&jsonValue.value, 0, valLen);
    memcpy(&jsonValue.value, value, strlen(value));

    jsonValue.type = type;

    return jsonValue;
}

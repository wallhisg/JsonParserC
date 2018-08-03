#include <json/json_writer.h>


bool json_write_key_value(Buffer *buff, JsonKeyValue keyValue)
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

JsonKeyValue json_create_key_value(JsonType type, char key[], char value[])
{
    JsonKeyValue keyValue;
    memset(&keyValue.key, 0, JSON_KEY_WRITE_LENGTH);
    memset(&keyValue.value, 0, JSON_VALUE_WRITE_LENGTH);

    keyValue.type = type;
    memcpy(&keyValue.key, key, strlen(key));
    memcpy(&keyValue.value, value, strlen(value));

    return keyValue;
}

#include <json/json_writer.h>


bool json_write_key_value(Buffer *buff, JsonKeyValue keyValue)
{


    if(json_write_key(buff, keyValue.key))
    {
        switch (keyValue.type) {
            case JSON_TYPE_STRING:
                json_write_value_type_string(buff, keyValue.value);
                break;
            case JSON_TYPE_OBJECT:
                json_write_value_type_object(buff, keyValue.value);
                break;

            case JSON_TYPE_ARRAY:

                break;
            default:
                break;
        }
    }
}

bool json_write_key(Buffer *buff, const char key[])
{
    buffer_write_one_byte(buff, '{');
    buffer_write_one_byte(buff, '\"');

    int len = strlen(key);
    printf("LEN %d\r\n", len);
    int i = 0;
    for(i = 0; i < len; ++i)
    {
        buffer_write_one_byte(buff, key[i]);
    }

    buffer_write_one_byte(buff, '\"');
    buffer_write_one_byte(buff, ':');
}

bool json_write_value_type_string(Buffer *buff, const char value[])
{
    buffer_write_one_byte(buff, '\"');

    int len = strlen(value);
    printf("LEN %d\r\n", len);
    int i = 0;
    for(i = 0; i < len; ++i)
    {
        buffer_write_one_byte(buff, value[i]);
    }

    buffer_write_one_byte(buff, '\"');
    buffer_write_one_byte(buff, '}');
    buffer_write_one_byte(buff, LF);
}

bool json_write_value_type_object(Buffer *buff, const char value[])
{

}

bool json_write_value_type_array(Buffer *buff, const char value[])
{

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

#ifndef JSON_WRITER_H
#define JSON_WRITER_H

#include <system/system.h>
#include <driver/buffer.h>
#include <json/json.h>


bool json_write_key_value(Buffer *buff, JsonKeyValue keyValue);

bool json_write_value_type_string(Buffer *buff, const char value[]);
bool json_write_value_type_object(Buffer *buff, const char value[]);
bool json_write_value_type_array(Buffer *buff, const char value[]);

bool json_write_key(Buffer *buff, const char key[]);

JsonKeyValue json_create_key_value(JsonType type, char key[], char value[]);

#endif  //  JSON_WRITER_H

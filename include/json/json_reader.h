#ifndef JSON_READ_H
#define JSON_READ_H

#include "json_parser.h"

JsonConsume json_read_frame(Buffer *inBuff);
JsonConsume json_read_key_value(Buffer *inBuff, JsonConsume *jsonConsume, JsonKeyValue *keyValue);

void json_read_sub_value();
char *json_get_key();
char *json_get_value();

#endif  //  JSON_READ_H

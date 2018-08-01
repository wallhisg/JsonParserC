#ifndef JSON_READ_H
#define JSON_READ_H

#include "json_parser.h"

JsonConsume json_read_key(Buffer *inBuff, JsonConsume *jsonConsume);
JsonConsume json_read_value(Buffer *inBuff, JsonConsume *jsonConsume);

char *json_get_key();
char *json_get_value();
char *json_get_string();

#endif  //  JSON_READ_H

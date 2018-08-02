#ifndef JSON_READ_H
#define JSON_READ_H

#include "json_parser.h"

JsonConsume json_read_key_value(Buffer *inBuff, JsonConsume *jsonConsume);

char *json_get_key();
char *json_get_value();

#endif  //  JSON_READ_H

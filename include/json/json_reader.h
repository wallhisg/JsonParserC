#ifndef JSON_READ_H
#define JSON_READ_H

#include "json_parser.h"

JsonConsume read_json_key(Buffer *inBuff, JsonConsume *jsonConsume);
JsonConsume read_json_value(Buffer *inBuff, JsonConsume *jsonConsume);

char *get_json_key();
char *get_json_value();

#endif  //  JSON_READ_H

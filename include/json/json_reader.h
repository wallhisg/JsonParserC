#ifndef JSON_READ_H
#define JSON_READ_H

#include "json_parser_object.h"


JsonConsume read_json_key(Buffer *inBuff, JsonConsume *jsonConsume);
JsonConsume read_json_value(Buffer *inBuff, JsonConsume *jsonConsume);

#endif  //  JSON_READ_H

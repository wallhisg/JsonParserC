#ifndef JSON_READ_H
#define JSON_READ_H

#include "json_parser.h"

JsonConsume json_read_frame(Buffer *inBuff);
JsonValue json_read_value(Buffer *inBuff, JsonConsume *jsonConsume);

void json_read_sub_value();

#endif  //  JSON_READ_H

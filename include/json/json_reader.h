#ifndef JSON_READ_H
#define JSON_READ_H

#include "json_parser_object.h"

typedef void* (ToKStart)(const char, void *);


char json_object_read_description(Buffer *inBuff);
char json_object_read_description_value(Buffer *inBuff);
char json_read_description_key(Buffer *inBuff, void *tokStart, JsonObjectState stateEnd);
char json_read_description_value(Buffer *inBuff, void *tokStart, JsonObjectState stateEnd);

void read_key_value(Buffer *inBuff);
void read_key(Buffer *inBuff, JsonConsume *objConsume);
void read_value(Buffer *inBuff, JsonConsume *objConsume);
#endif  //  JSON_READ_H

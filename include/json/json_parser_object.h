#ifndef JSON_PARSER_OBJECT_H
#define JSON_PARSER_OBJECT_H

#include <stdbool.h>
#include <json/json.h>



bool json_parser_object(Buffer *inBuff);
JsonConsume consume_object(const char c, JsonConsume *objConsume);




#endif // JSON_PARSER_OBJECT_H

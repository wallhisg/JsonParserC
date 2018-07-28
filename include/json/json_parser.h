#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <stdbool.h>
#include <ctype.h>
#include <json/json.h>
#include <driver/buffer.h>

JsonType get_json_type(Buffer *inBuff);
JsonConsume consume_char(char c, JsonConsume *jsonConsume );

#endif // JSON_PARSER_H

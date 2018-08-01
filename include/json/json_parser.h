#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <stdbool.h>
#include <ctype.h>
#include <json/json.h>
#include <driver/buffer.h>


JsonConsume get_json_type(Buffer *inBuff, JsonConsume *jsonConsume);
JsonConsume consume_char(const char c, JsonConsume *objConsume);

#endif // JSON_PARSER_H

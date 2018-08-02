#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <stdbool.h>
#include <ctype.h>
#include <json/json.h>
#include <driver/buffer.h>

JsonConsume consume_char(const char c, JsonConsume *consume);

#endif // JSON_PARSER_H

#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <stdbool.h>
#include <ctype.h>
#include <json/json.h>
#include <driver/buffer.h>

JsonConsume tok_letter_start(const char c, JsonConsume *jsonConsume);
JsonConsume tok_l_curly(const char c, JsonConsume *jsonConsume);
JsonConsume tok_r_curly(const char c, JsonConsume *jsonConsume);
JsonConsume tok_l_bracket(const char c, JsonConsume *jsonConsume);
JsonConsume tok_r_bracket(const char c, JsonConsume *jsonConsume);
JsonConsume tok_colon(const char c, JsonConsume *jsonConsume);
JsonConsume tok_comma(const char c, JsonConsume *jsonConsume);
JsonConsume tok_dq_mark(const char c, JsonConsume *jsonConsume);
JsonConsume tok_letter(const char c, JsonConsume *jsonConsume);

JsonConsume get_json_type(Buffer *inBuff, JsonConsume *jsonConsume);
JsonConsume consume_char(char c, JsonConsume *jsonConsume );

#endif // JSON_PARSER_H

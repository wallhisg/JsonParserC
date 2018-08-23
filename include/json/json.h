#ifndef JSON_H
#define JSON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include <system/system.h>
#include <driver/buffer_ring.h>

#define LF      0x0A    //  '\n' - New Line
#define CR      0x0D    //  '\r' - Enter

#define JSON_KEY_WRITE_LENGTH     8
#define JSON_VALUE_WRITE_LENGTH   32

typedef enum {
    JSON_TYPE_UNDEFINED = 00,
    JSON_TYPE_STRING    = 01,
    JSON_TYPE_OBJECT    = 02,
    JSON_TYPE_ARRAY     = 03,
    JSON_TYPE_RAW       = 04
} JsonType;

typedef enum {
    JSON_STATE_UNDEFINE             = 00,   // 00
    JSON_STATE_BEGIN                = 01,   // 01
    JSON_STATE_NAME_BEGIN           = 02,   // 02
    JSON_STATE_NAME                 = 03,   // 03
    JSON_STATE_NAME_END             = 04,   // 04

    JSON_STATE_VALUE_BEGIN          = 05,   // 05

    JSON_STATE_VALUE_STR_BEGIN      = 16,   // 06
    JSON_STATE_VALUE_STRING         = 17,
    JSON_STATE_VALUE_STR_END        = 18,   // 07

    JSON_STATE_VALUE_OBJECT_BEGIN   = 26,   // 16
    JSON_STATE_VALUE_OBJECT         = 27,
    JSON_STATE_VALUE_OBJECT_END     = 28,   // 17

    JSON_STATE_VALUE_ARRAY_BEGIN    = 36,   // 26
    JSON_STATE_VALUE_ARRAY          = 37,
    JSON_STATE_VALUE_ARRAY_END      = 38,   // 27

    JSON_STATE_VALUE_END            = 9,    // 08
    JSON_END                        = 10    // 09
} JsonState;

typedef struct JsonConsume{
    JsonState state;
    TriBool tribool;
    JsonType type;
    struct JsonConsume (*nextTok)(const char, struct JsonConsume *);
} JsonConsume;


typedef struct {
    char *name;
    char *value;
} JsonObject;

void json_init();
Buffer *get_json_buffer();
void json_consume_init(JsonConsume *jsonConsume);

#endif // JSON_H

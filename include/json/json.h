#ifndef JSON_H
#define JSON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include <system/system.h>
#include <driver/ring_buffer.h>

#define LF      0x0A    //  '\n' - New Line
#define CR      0x0D    //  '\r' - Enter

#define JSON_KEY_WRITE_LENGTH     8
#define JSON_VALUE_WRITE_LENGTH   32

typedef enum {
    JSON_TYPE_UNDEFINED,
    JSON_TYPE_STRING,
    JSON_TYPE_OBJECT,
    JSON_TYPE_ARRAY,
    JSON_TYPE_RAW
} JsonType;

typedef enum {
    // States of object and array
    JSON_STATE_UNDEFINE = 00,           // 00
    JSON_STATE_BEGIN = 01,              // 01
    JSON_STATE_NAME_BEGIN = 02,         // 02
    JSON_STATE_NAME = 03,               // 03
    JSON_STATE_NAME_END = 04,           // 04
    JSON_STATE_VALUE = 05,              // 05
    JSON_STATE_VALUE_STR_BEGIN = 06,    // 06
    JSON_STATE_VALUE_STR_END = 07,      // 07
    JSON_STATE_VALUE_OBJECT_BEGIN = 16, // 16
    JSON_STATE_VALUE_OBJECT_END = 17,   // 17
    JSON_STATE_VALUE_ARRAY_BEGIN = 26,  // 26
    JSON_STATE_VALUE_ARRAY_END = 27,    // 27
    JSON_STATE_END = 8,                 // 08
    JSON_END = 9                        // 09
    // state of string
} JsonState;

typedef struct JsonConsume{
    JsonState state;
    TriBool tribool;
    JsonType type;
    struct JsonConsume (*nextTok)(const char, struct JsonConsume *);
} JsonConsume;

typedef struct {
    JsonType type;
    char key[JSON_KEY_WRITE_LENGTH];
    char value[JSON_VALUE_WRITE_LENGTH];
} JsonKeyValue;

void json_init();
Buffer *get_json_buffer();
void json_consume_init(JsonConsume *jsonConsume);

#endif // JSON_H

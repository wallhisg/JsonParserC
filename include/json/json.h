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

// SYSTEM MESSAGE
// define TYPE JSON
#define MAX_NO_OF_JSON_OBJECT   0x08
#define MSG_TYPE_JSON           0x80
#define MSG_TYPE_JSON_STRING    0x81
#define MSG_TYPE_JSON_OBJECT    0x82
#define MSG_TYPE_JSON_ARRAY     0x83

#define MAX_NO_OF_JSON_BUFFER       128
#define JSON_OBJECT_KEY_LENGTH      64
#define JSON_OBJECT_VALUE_LENGTH    64

typedef enum {
    ERROR_NONE      = 0,
    ERROR_SYNTAX    = 1,
    ERROR_UNDEFINED = 2
} JSonError;

typedef enum {
    JSON_TYPE_UNDEFINED,
    JSON_TYPE_STRING,
    JSON_TYPE_OBJECT,
    JSON_TYPE_ARRAY,
    JSON_TYPE_RAW
} JsonType;

typedef enum {
    // States of object and array
    JSON_STATE_UNDEFINE = 00,  // 0
    JSON_STATE_BEGIN = 01,          // 1
    JSON_STATE_NAME_BEGIN = 02,            // 2
    JSON_STATE_NAME = 03,      // 3
    JSON_STATE_NAME_END = 04,        // 4
    JSON_STATE_VALUE = 05,          // 5
    JSON_STATE_VALUE_STR_BEGIN = 06,    // 6
    JSON_STATE_VALUE_STR_END = 07,      // 7
    JSON_STATE_VALUE_OBJECT_BEGIN = 16,    // 6
    JSON_STATE_VALUE_OBJECT_END = 17,      // 7
    JSON_STATE_VALUE_ARRAY_BEGIN = 26,    // 6
    JSON_STATE_VALUE_ARRAY_END = 27,      // 7
    JSON_STATE_END = 8,             // 8
    JSON_END = 9
    // state of string
} JsonState;

// six struct tokens
typedef enum {
    JTK_L_CURLY,        //  {
    JTK_R_CURLY,        //  }
    JTK_COLON,          //  :
    JTK_COMMA,          //  ,
    JTK_L_BRACKET,      //  [
    JTK_R_BRACKET,      //  ]
    // extra token
    JEX_LETTER_START,   //
    JEX_DQ_MARK,        //  "
    JEX_LETTER,         //  letter
    JEX_SPACE,          //  space
} JsonToken;

typedef struct {
    JsonToken tok;
    TriBool tribool;
    JsonType type;
} JSonTupleFlag;

typedef char *JsonString;

typedef struct {
    char key[JSON_OBJECT_KEY_LENGTH];
    char value[JSON_OBJECT_VALUE_LENGTH];
} JsonValue, *JsonValuePtr;

typedef struct {
    JsonValue keyValue;
    bool isUsed;
} JsonObject, *JsonObjectPtr;

typedef struct {
    JsonObjectPtr jsonObj;
    size_t size;
} JsonObjectArray, *JsonObjectArrayPtr;

typedef struct JsonConsume{
    JsonState state;
    TriBool tribool;
    JsonType type;
    struct JsonConsume (*nextTok)(const char, struct JsonConsume *);
} JsonConsume;

void json_init();
void write_one_byte_to_json_buffer(char byte);
char read_one_byte_from_json_buffer();

Buffer *get_json_buffer();

void clear_buffer(char *buff, size_t size);

bool is_tok_letter(const char c);

bool insert_json_object(JsonValue keyValue);
JsonValue *get_json_object();

int find_free_json_object_slot();
int get_no_of_json_object();
void clear_json_object(JsonValue *keyValue);
bool is_json_object_empty(JsonObject jsonObj);
void json_consume_init(JsonConsume *jsonConsume);

#endif // JSON_H

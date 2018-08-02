#include <json/json.h>
#include <string.h>

#define JSON_BUFFER_LENGTH       128

static char JSON_BUFFER[JSON_BUFFER_LENGTH];
static Buffer jsonBuff;

void json_init()
{
    // init json buffer
    jsonBuff.buffer = JSON_BUFFER;
    jsonBuff.size = JSON_BUFFER_LENGTH;
    jsonBuff.head = jsonBuff.tail = jsonBuff.buffer;
    buffer_reset(&jsonBuff);
}

Buffer *get_json_buffer()
{
    return &jsonBuff;
}

void json_consume_init(JsonConsume *jsonConsume)
{
    jsonConsume->state = JSON_STATE_UNDEFINE;
    jsonConsume->tribool =  TRIBOOL_INDETERMINATE;
    jsonConsume->type =  JSON_TYPE_RAW;
}

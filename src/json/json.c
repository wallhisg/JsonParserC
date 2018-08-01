#include <json/json.h>
#include <string.h>

static char JSON_BUFFER[MAX_NO_OF_JSON_BUFFER];

static JsonObject JSON_OBJECT[MAX_NO_OF_JSON_OBJECT];
static JsonObjectArray objectArray;

static Buffer jsonBuff;

void json_init()
{
    clear_buffer(JSON_BUFFER, MAX_NO_OF_JSON_BUFFER);

    // init json buffer
    jsonBuff.buffer = JSON_BUFFER;
    jsonBuff.size = MAX_NO_OF_JSON_BUFFER;
    jsonBuff.head = jsonBuff.tail = jsonBuff.buffer;
    buffer_reset(&jsonBuff);;
    
    // init json object
    memset(JSON_OBJECT, 0, MAX_NO_OF_JSON_OBJECT * sizeof(JsonObject));

    objectArray.jsonObj = JSON_OBJECT;
    objectArray.size = MAX_NO_OF_JSON_OBJECT;
    
    
}

Buffer *get_json_buffer()
{
    return &jsonBuff;
}

void write_one_byte_to_json_buffer(char byte)
{
    buffer_write_one_byte(&jsonBuff, byte);
}

char read_one_byte_from_json_buffer()
{
    return buffer_read_one_byte(&jsonBuff);
}

void clear_buffer(char *buff, size_t size)
{
    memset(buff, 0, size);
}

bool is_tok_letter(const char c)
{
    bool flag = false;
    switch (c)
    {
        case '{': case '}': case '\"': case '[': case ']':
        {
            flag = true;
            break;
        }
        default:
            flag = false;
    }

    return flag;
}

bool insert_json_object(JsonValue keyValue)
{
    bool flag = false;
   printf("insert_json_object\r\n");
//    printf("Key: %s\r\n", keyValue.key);
//    printf("val: %s\r\n", keyValue.value);

    // find free json object slot
    int objSlot = find_free_json_object_slot();
//    printf("slot found: %d\r\n", objSlot);

    if(objSlot >= 0)
    {
        JsonObject jsonObj ={0};
        jsonObj.keyValue = keyValue;
        jsonObj.isUsed = true;
        objectArray.jsonObj[objSlot] = jsonObj;

        flag = true;
    }
    return flag;
}

int find_free_json_object_slot()
{
    int objSlot = -1;
    int slotFound = 0;

    int i = 0;
    for(i = 0; (i < objectArray.size) && (slotFound == 0); i++)
    {
        if(!objectArray.jsonObj[i].isUsed)
        {
            objSlot = i;
            slotFound = 1;
        }
    }
    return objSlot;
}

JsonValue *get_json_object()
{
    JsonValue *result = NULL;
    JsonObject *objs = objectArray.jsonObj;

    int counter = get_no_of_json_object();
    if(counter)
    {
        int i = 0;
        for(i = 0; i < MAX_NO_OF_JSON_OBJECT; i++)
        {
            if(objs[i].isUsed)
            {
                result = &objs[i].keyValue;
                break;
            }

        }
    }
    return result;
}

void clear_json_object(JsonValue *keyValue)
{
    JsonObjectPtr objs = objectArray.jsonObj;

    int i = 0;
    for(i = 0; i < objectArray.size; i++)
    {
        if((objs[i].isUsed) && objs[i].keyValue.key == keyValue->key)
        {
            objs[i].isUsed = false;
        }
    }
}

int get_no_of_json_object()
{
    int slots_ = 0;
    JsonObjectPtr objs = objectArray.jsonObj;

    int i =0;
    for(i = 0; i < objectArray.size; i++)
    {
        if(objs[i].isUsed)
            slots_++;
    }
    return slots_;
}

void json_consume_init(JsonConsume *jsonConsume)
{
    jsonConsume->tribool =  TRIBOOL_INDETERMINATE;
    jsonConsume->type =  JSON_TYPE_RAW;
    jsonConsume->state = JSON_STATE_UNDEFINE;
}

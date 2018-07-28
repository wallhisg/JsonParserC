#include <json/json_reader.h>

extern JsonConsume tok_obj_letter_start(const char c, JsonConsume objConsume);
extern JsonConsume tok_obj_dq_mark(const char c, JsonConsume objConsume);
extern JsonConsume tok_obj_colon(const char c, JsonConsume objConsume);
extern JsonConsume tok_obj_comma(const char c, JsonConsume objConsume);
extern JsonConsume tok_obj_letter(const char c, JsonConsume objConsume);
extern JsonConsume tok_obj_l_curly(const char c, JsonConsume objConsume);
extern JsonConsume tok_obj_r_curly(const char c, JsonConsume objConsume);
extern JsonConsume tok_obj_l_bracket(const char c, JsonConsume objConsume);
extern JsonConsume tok_obj_r_bracket(const char c, JsonConsume objConsume);

void read_key_value(Buffer *inBuff)
{
    printf("Buffer input: %s\r\n", inBuff->buffer);
    JsonConsume consume;
    json_consume_init(&consume);
    consume.nextTok = (void*)tok_obj_letter_start;
    consume.state = JSON_OBJECT_BEGIN;
    
    read_key(inBuff, &consume);
    read_value(inBuff, &consume);
    read_key(inBuff, &consume);
    read_value(inBuff, &consume);
}

void read_key(Buffer *inBuff, JsonConsume *objConsume)
{
    char byte = ' ';
    JsonConsume consume;
    int i = 0;
    for(i = 0; i < inBuff->size; ++i)
    {
        byte = buffer_read_one_byte(inBuff);
        consume = consume_object(byte, objConsume);
        
        if ((consume.tribool == TRIBOOL_TRUE) && (consume.state == JSON_OBJECT_KEY_BEGIN))
        {
            printf("%c", byte);
        }
        if(consume.state == JSON_OBJECT_VALUE)
            break;
    }
    printf("\r\n");
    printf("Buffer remain: %s\r\n", inBuff->buffer);
}
void read_value(Buffer *inBuff, JsonConsume *objConsume)
{
    char byte = ' ';
    JsonConsume consume;
    int i = 0;
    for(i = 0; i < inBuff->size; ++i)
    {
        byte = buffer_read_one_byte(inBuff);
        consume = consume_object(byte, objConsume);
        
        if ((consume.tribool == TRIBOOL_TRUE) && (consume.state == JSON_OBJECT_VALUE_BEGIN))
        {
            printf("%c", byte);
        }
        if(consume.state == JSON_OBJECT_VALUE_END)
            break;
    }
    printf("\r\n");
    printf("Buffer remain: %s\r\n", inBuff->buffer);
}

char json_object_read_description(Buffer *inBuff)
{
//     return json_read_description_key(inBuff, tok_obj_letter_start, JSON_OBJECT_VALUE);
    return json_read_description_key(inBuff, tok_obj_letter_start, JSON_OBJECT_VALUE);
}

char json_object_read_description_value(Buffer *inBuff)
{
//     return json_read_description_value(inBuff, tok_obj_letter_start, JSON_OBJECT_VALUE);
    return json_read_description_value(inBuff, tok_obj_letter_start, JSON_OBJECT_KEY);
}

char json_read_description_key(Buffer *inBuff, void *tokStart, JsonObjectState stateEnd)
{
    printf("json_obJect_read_description\r\n");
    printf("Buffer input: %s\r\n", inBuff->buffer);
    JsonConsume consume;
    json_consume_init(&consume);
    
    consume.nextTok = tokStart;
    consume.state = JSON_OBJECT_BEGIN;
    
    char desKey = 'a';
//     char byte = ' ';
    
//     int i = 0;
//     for(i = 0; i < inBuff->size; ++i)
//     {
// //         printf("%c\r\n", byte);
// //         printf("consume.state: %d\r\n", consume.state);
//         byte = buffer_read_one_byte(inBuff);
//         consume = consume_object(byte, consume);
//         if( consume.state == JSON_OBJECT_KEY_BEGIN)
//         {
//             desKey = byte;
// //             printf("%c\r\n", description);
// //             break;
//         }
//         if(consume.state == stateEnd)
//             break;
//     }
    printf("description key: %c\r\n", desKey );
    printf("Buffer remain: %s\r\n", inBuff->buffer);
    return desKey;
}

char json_read_description_value(Buffer *inBuff, void *tokStart, JsonObjectState stateEnd)
{
    printf("json_obJect_read_description\r\n");
    printf("Buffer input: %s\r\n", inBuff->buffer);
    JsonConsume consume;
    json_consume_init(&consume);
    
    consume.nextTok = tokStart;
    consume.state = JSON_OBJECT_BEGIN;
    
    char desValue = 'a';
//     char byte = ' ';
    
//     int i = 0;
//     for(i = 0; i < inBuff->size; ++i)
//     {
// //         printf("%c\r\n", byte);
// //         printf("consume.state: %d\r\n", consume.state);
//         byte = buffer_read_one_byte(inBuff);
//         consume = consume_object(byte, consume);
//         if( consume.state == JSON_OBJECT_KEY_BEGIN)
//         {
//             desValue = byte;
// //             printf("%c\r\n", description);
// //             break;
//         }
//         if(consume.state == stateEnd)
//             break;
//     }
    printf("description value: %c\r\n", desValue);
    printf("Buffer remain: %s\r\n", inBuff->buffer);
    return desValue;

}

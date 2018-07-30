#include <json/json_parser.h>

JsonConsume tok_letter_start(const char c, JsonConsume *objConsume);
JsonConsume tok_dq_mark(const char c, JsonConsume *objConsume);
JsonConsume tok_colon(const char c, JsonConsume *objConsume);
JsonConsume tok_comma(const char c, JsonConsume *objConsume);
JsonConsume tok_left_curly(const char c, JsonConsume *objConsume);
JsonConsume tok_right_curly(const char c, JsonConsume *objConsume);
JsonConsume tok_left_bracket(const char c, JsonConsume *objConsume);
JsonConsume tok_right_bracket(const char c, JsonConsume *objConsume);
JsonConsume tok_letter(const char c, JsonConsume *objConsume);
JsonObjectState next_json_object_state(JsonObjectState objState);

JsonConsume get_json_type(Buffer *inBuff, JsonConsume *jsonConsume)
{
    debug_json("BUFFER SIZE %d\r\n", inBuff->size);
    jsonConsume->nextTok = (void *)tok_letter_start;

    Buffer *jsonBuff = get_json_buffer();
	JsonConsume consume;
    int counter = 0;
    char byte;
    int i = 0;
    for(i = 0; i < inBuff->size; i++)
    {
        // process array char from buffer
        byte = buffer_read_one_byte(inBuff);
        consume = consume_char(byte, jsonConsume);

        if(consume.tribool == TRIBOOL_TRUE) {
            if(jsonBuff->status != RING_STATUS_FULL) {
                write_one_byte_to_json_buffer(byte);
                counter++;
            }
            else {
                consume.tribool = TRIBOOL_FALSE;
                consume.type = JSON_TYPE_UNDEFINED;
                // read buffer continuously until get LF
                consume_buffer(jsonBuff, LF);
                consume_buffer(inBuff, LF);
                error("JSON BUFFER IS FULL");
                break;
            }
        }
        else if(consume.tribool == TRIBOOL_FALSE) {
            debug("Json string false at %d.\r\n", i);
            consume.type = JSON_TYPE_UNDEFINED;
            // read buffer continuously until get LF
            consume_buffer(jsonBuff, LF);
            consume_buffer(inBuff, LF);
            break;
        }
        // end block; end of json frame \n
        if(byte == LF) {
            counter++;
            consume.counter = counter;
            break;
        }
    }
    return consume;
}

JsonConsume consume_char(const char c, JsonConsume *objConsume)
{
    JsonConsume consume;
    consume = objConsume->nextTok(c, objConsume);
    return consume;
}

JsonConsume tok_letter_start(const char c, JsonConsume *objConsume)
{
    debug_message("tok_obj_letter_start\r\n");
    JsonConsume *consume = objConsume;

    switch (c) {
        case '\"' :
        {
            consume->nextTok = (void *)tok_dq_mark;
            if(consume->state != JSON_OBJECT_INDERERMINATE) {
                consume->tribool = TRIBOOL_INDETERMINATE;
                consume->state = next_json_object_state(consume->state);
            }
            else {
                consume->tribool = TRIBOOL_INDETERMINATE;
                consume->type = JSON_TYPE_STRING;
            }
            break;
        }
        case '{': {
            consume->nextTok = (void *)tok_left_curly;
            consume->tribool = TRIBOOL_INDETERMINATE;
            consume->type = JSON_TYPE_OBJECT;
            consume->counter++;
            break;
        }
        case LF:
        {
            consume->tribool = TRIBOOL_INDETERMINATE;
            consume->type = JSON_TYPE_UNDEFINED;
            consume->nextTok = (void *)tok_letter_start;
            break;
        }
        default: {
            consume->tribool = TRIBOOL_FALSE;
            break;
        }
    }

    return *consume;
}

// CASE "
JsonConsume tok_dq_mark(const char c, JsonConsume *objConsume)
{
    debug_message("tok_obj_dq_mark");
    JsonConsume *consume = objConsume;

    switch (c) {
        case ':' : {
            consume->nextTok = (void *)tok_colon;
            if(consume->state != JSON_OBJECT_INDERERMINATE) {
                if(consume->isObjects) {
                    consume->tribool = TRIBOOL_TRUE;
                }
                else {
                    consume->state = next_json_object_state(consume->state);
                    consume->tribool = TRIBOOL_INDETERMINATE;
                }
            }
            else {
                consume->tribool = TRIBOOL_TRUE;
            }

            break;
        }
        case ',': {
            consume->nextTok = (void *)tok_comma;
            if(consume->state != JSON_OBJECT_INDERERMINATE) {
                if(consume->isObjects) {
                    consume->tribool = TRIBOOL_TRUE;
                }
                else {
                    consume->state = next_json_object_state(consume->state);
                    consume->tribool = TRIBOOL_INDETERMINATE;
                }
            }
            else {
                consume->nextTok = (void *)tok_comma;
                consume->tribool = TRIBOOL_TRUE;
            }
                
            break;
        }
        case '}': {
            consume->nextTok = (void *)tok_right_curly;
            if(consume->state != JSON_OBJECT_INDERERMINATE) {
                consume->isObjects = false;
                consume->state = next_json_object_state(consume->state);
                consume->tribool = TRIBOOL_INDETERMINATE;
            }
            else {
                consume->counter--;
                if(consume->isObjects) {
                    consume->tribool = TRIBOOL_TRUE;
                    consume->isObjects = false;
                }
                else
                    consume->tribool = TRIBOOL_INDETERMINATE;
            }
            break;
        }
        case ']' : {
            consume->nextTok = (void *)tok_right_bracket;
            if(consume->state != JSON_OBJECT_INDERERMINATE) {
                consume->state = next_json_object_state(consume->state);
                consume->tribool = TRIBOOL_INDETERMINATE;
                consume->isObjects = false;
            }
            else {
                consume->isObjects = false;
                consume->tribool = TRIBOOL_TRUE;
                consume->counter--;
            }
            break;
        }
        case '\0':
        case '\r':
        case '\n':
        {
            consume->tribool = TRIBOOL_TRUE;
            consume->state = JSON_OBJECT_END;
            break;
        }
        default: {
            if(consume->state != JSON_OBJECT_INDERERMINATE) {
                if(isalpha(c) || isdigit(c)) {
                    consume->nextTok = (void *)tok_letter;
                    consume->tribool = TRIBOOL_TRUE;
                    if(consume->state == JSON_OBJECT_KEY)
                        consume->state = JSON_OBJECT_KEY_BEGIN;
                    else if(consume->state == JSON_OBJECT_VALUE)
                        consume->state = JSON_OBJECT_VALUE_BEGIN;
                }
                else {
                    consume->tribool = TRIBOOL_FALSE;
                }
            }
            else {
                if (isalpha(c) || isdigit(c)) {
                    consume->nextTok = (void *)tok_letter;
                    consume->tribool = TRIBOOL_TRUE;
                }
                else if ((c == '\0') || (c == '\r') || (c == '\n')) {
                    consume->tribool = TRIBOOL_TRUE;
                }
                else {
                    consume->tribool = TRIBOOL_FALSE;
                }
            }
            break;
        }
    }

    return *consume;
}

// CASE :
JsonConsume tok_colon(const char c, JsonConsume *objConsume)
{
    debug_message("tok_obj_colon");

    JsonConsume *consume = objConsume;
    
    switch (c) {
        case '\"' : {
            consume->nextTok = (void *)tok_dq_mark;
            if(consume->state != JSON_OBJECT_INDERERMINATE) {
                if(consume->isObjects) {
                    consume->tribool = TRIBOOL_TRUE;
                }
                else {
                    consume->tribool = TRIBOOL_INDETERMINATE;
                }
            }
            else {
                consume->tribool = TRIBOOL_TRUE;
            }
            break;
        }
        case '{':
        {
            consume->nextTok = (void *)tok_left_curly;
            if(consume->state != JSON_OBJECT_INDERERMINATE) {
                consume->isObjects = true;
                consume->state = next_json_object_state(consume->state);
                consume->tribool = TRIBOOL_INDETERMINATE;
            }
            else {
                consume->isObjects = true;
                consume->tribool = TRIBOOL_TRUE;
                consume->counter++;
            }
            break;
        }
        case '[' :
        {
            consume->nextTok = (void *)tok_left_bracket;

            if(consume->state != JSON_OBJECT_INDERERMINATE) {
                consume->isObjects = true;
                consume->state = next_json_object_state(consume->state);
                consume->tribool = TRIBOOL_INDETERMINATE;
            }
            else {
                consume->tribool = TRIBOOL_TRUE;
                consume->type = JSON_TYPE_ARRAY;
                consume->isObjects = true;
                consume->counter++;
            }
            break;
        }
        default: {
            consume->tribool = TRIBOOL_FALSE;
            break;
        }
    }

    return *consume;
}

// CASE ,
JsonConsume tok_comma(const char c, JsonConsume *objConsume)
{
    debug_message("tok_obj_comma");

    JsonConsume *consume = objConsume;
    
    switch (c) {
        case '\"' :
        {
            consume->nextTok = (void *)tok_dq_mark;
            if(consume->state != JSON_OBJECT_INDERERMINATE) {
                if(consume->isObjects) {
                    consume->tribool = TRIBOOL_TRUE;
                }
                else {
                    consume->state = next_json_object_state(consume->state);
                    consume->tribool = TRIBOOL_INDETERMINATE;
                }
            }
            else {
                consume->tribool = TRIBOOL_TRUE;
            }
            break;
        }
        default: {
            consume->tribool = TRIBOOL_FALSE;
            break;
        }
    }

    return *consume;
}

// CASE {
JsonConsume tok_left_curly(const char c, JsonConsume *objConsume)
{
    debug_message("tok_obj_l_curly");

    JsonConsume *consume = objConsume;
    
    switch (c) {
        case '\"' :
        {
            consume->nextTok = (void *)tok_dq_mark;
            if(consume->state != JSON_OBJECT_INDERERMINATE) {
                if(consume->isObjects) {
                    consume->tribool = TRIBOOL_TRUE;
                }
            }
            else {
                consume->tribool = TRIBOOL_TRUE;
            }
            break;
        }
        default: {
            consume->tribool = TRIBOOL_FALSE;
            break;
        }
    }

    return *consume;
}

// CASE }
JsonConsume tok_right_curly(const char c, JsonConsume *objConsume)
{
    debug_message("tok_obj_r_curly");

    JsonConsume *consume = objConsume;
    
    switch (c) {
        case ',' :
        {
            consume->nextTok = (void *)tok_comma;
            if(consume->state != JSON_OBJECT_INDERERMINATE) {
                consume->isObjects = false;
                consume->tribool = TRIBOOL_INDETERMINATE;
                consume->state = next_json_object_state(consume->state);
            }
            else {
                consume->tribool = TRIBOOL_TRUE;
            }
            break;
        }
        case '}':
        {
            consume->nextTok = (void *)tok_right_curly;
            consume->counter--;

            if (consume->counter > 1) {
                consume->tribool = TRIBOOL_TRUE;
            }
            else {
                consume->tribool = TRIBOOL_INDETERMINATE;
            }
            break;
        }
        case '\0':
        case '\r':
        case '\n':
        {
            consume->tribool = TRIBOOL_TRUE;
            consume->state = JSON_OBJECT_INDERERMINATE;
            break;
        }
        default: {
            consume->tribool = TRIBOOL_FALSE;
            break;
        }
    }

    return *consume;
}

//  CASE [
JsonConsume tok_left_bracket(const char c, JsonConsume *objConsume)
{
    debug_message("tok_obj_l_bracket");

    JsonConsume *consume = objConsume;
    
    switch (c) {
        case '\"' :
        {
            consume->nextTok = (void *)tok_dq_mark;
            if(consume->state != JSON_OBJECT_INDERERMINATE) {
                if(consume->isObjects) {
                    consume->tribool = TRIBOOL_TRUE;
                }
            }
            else {
                consume->isObjects = true;
                consume->tribool = TRIBOOL_TRUE;
            }
            break;
        }
        default: {
            consume->tribool = TRIBOOL_FALSE;
            break;
        }
    }

    return *consume;
}

//  CASE ]
JsonConsume tok_right_bracket(const char c, JsonConsume *objConsume)
{
    debug_message("tok_obj_r_bracket");

    JsonConsume *consume = objConsume;
    
    switch (c) {
        case '\0':
        case '\r':
        case '\n':
        {
            consume->tribool = TRIBOOL_TRUE;
            consume->state = JSON_OBJECT_END;
            break;
        }
        case ',' :
        {
            consume->nextTok = (void *)tok_comma;
            if(consume->state != JSON_OBJECT_INDERERMINATE) {
                consume->isObjects = false;
                consume->tribool = TRIBOOL_INDETERMINATE;
                consume->state = next_json_object_state(consume->state);
            }
            else {
                consume->tribool = TRIBOOL_TRUE;
            }
            break;
        }
        case '}':
        {
            consume->nextTok = (void *)tok_right_curly;
            consume->isObjects = false;
            consume->tribool = TRIBOOL_INDETERMINATE;
            consume->state = next_json_object_state(consume->state);
            break;
        }
        default: {
            consume->tribool = TRIBOOL_FALSE;
            break;
        }
    }

    return *consume;
}

// CASE A -> Z, 0 -> 1
JsonConsume tok_letter(const char c, JsonConsume *objConsume)
{
    debug_message("tok_obj_letter");
    JsonConsume *consume = objConsume;
    
    switch (c) {
        case '\"' :
        {
            consume->nextTok = (void *)tok_dq_mark;
            if(consume->state != JSON_OBJECT_INDERERMINATE) {
                if(consume->isObjects) {
                    consume->tribool = TRIBOOL_TRUE;
                }
                else {
                    consume->state = next_json_object_state(consume->state);
                }
            }
            else if (consume->type == JSON_TYPE_STRING) {
                consume->nextTok = (void *)tok_dq_mark;
                consume->tribool = TRIBOOL_INDETERMINATE;
            }
            else if (consume->type == JSON_TYPE_OBJECT) {
                consume->nextTok = (void *)tok_dq_mark;
                consume->tribool = TRIBOOL_TRUE;
            }
            else if (consume->type == JSON_TYPE_ARRAY) {
                consume->nextTok = (void *)tok_dq_mark;
                consume->tribool = TRIBOOL_TRUE;
            }
            else {
                consume->nextTok = (void *)tok_dq_mark;
                consume->tribool = TRIBOOL_INDETERMINATE;
            }
            break;
        }
        default: {
            if (isalpha(c) || isdigit(c)) {
                consume->nextTok = (void *)tok_letter;
                consume->tribool = TRIBOOL_TRUE;
            }
            else
                consume->tribool = TRIBOOL_FALSE;

            break;
        }
    }

    return *consume;
}

JsonObjectState next_json_object_state(JsonObjectState objState)
{
    JsonObjectState state = objState;
    switch (state)
    {
        case JSON_OBJECT_INDERERMINATE :
            state = JSON_OBJECT_BEGIN;
            break;
        case JSON_OBJECT_BEGIN:
            state = JSON_OBJECT_KEY;
            break;
        case JSON_OBJECT_KEY:
            state = JSON_OBJECT_KEY_BEGIN;
            break;            
        case JSON_OBJECT_KEY_BEGIN:
            state = JSON_OBJECT_KEY_END;
            break;
        case JSON_OBJECT_KEY_END:
            state = JSON_OBJECT_VALUE;
            break;
        case JSON_OBJECT_VALUE:
            state = JSON_OBJECT_VALUE_BEGIN;
            break;
        case JSON_OBJECT_VALUE_BEGIN:
            state = JSON_OBJECT_VALUE_END;
            break;
        case JSON_OBJECT_VALUE_END:
            state = JSON_OBJECT_BEGIN;
            break;
        default:
            state = JSON_OBJECT_INDERERMINATE;
            break;
    }
    return state;
}

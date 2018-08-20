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
JsonConsume tok_ctrl_letter(const char c, JsonConsume *objConsume);

#ifndef CONSUME_DEBUG
    #define CONSUME_DEBUG   0
#endif

JsonConsume consume_char(const char c, JsonConsume *consume)
{
    consume->nextTok(c, consume);
#if CONSUME_DEBUG
    printf("TYPE: %d - TRIBOOL: %d - CHAR %c - STATE %02d\r\n", consume->type, consume->tribool, c, consume->state);
#endif
    return *consume;
}

JsonConsume tok_letter_start(const char c, JsonConsume *objConsume)
{
    debug_message("tok_obj_letter_start\r\n");
    JsonConsume *consume = objConsume;

    switch (c) {
        case '{': {
            consume->nextTok = (void *)tok_left_curly;
            consume->state = JSON_STATE_BEGIN;
            consume->tribool = TRIBOOL_TRUE;
            break;
        }
        case CR:
        case LF:
        case '\0': {
            consume->nextTok = (void *)tok_ctrl_letter;
            consume->tribool = TRIBOOL_INDETERMINATE;
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

            if(consume->state == JSON_STATE_NAME_END) {
                consume->state = JSON_STATE_VALUE_BEGIN;
                consume->tribool = TRIBOOL_TRUE;
            }
            else if(consume->state == JSON_STATE_VALUE_OBJECT) {
                consume->tribool = TRIBOOL_TRUE;
            }
            else
                consume->tribool = TRIBOOL_FALSE;

            break;
        }
        case ',': {
            consume->nextTok = (void *)tok_comma;

            if(consume->state == JSON_STATE_VALUE_OBJECT)
            {
                consume->tribool = TRIBOOL_TRUE;
            }
            else if(consume->state == JSON_STATE_VALUE_ARRAY)
            {
                consume->tribool = TRIBOOL_TRUE;
            }
            else if(consume->state == JSON_STATE_VALUE_STR_END) {
                consume->state = JSON_STATE_VALUE_END;
                consume->tribool = TRIBOOL_TRUE;
            }                    
            else
                consume->tribool = TRIBOOL_FALSE;

            break;
        }
        case '}': 
        {
            consume->nextTok = (void *)tok_right_curly;

            if(consume->state == JSON_STATE_VALUE_OBJECT) {
                consume->state = JSON_STATE_VALUE_OBJECT_END;
                consume->tribool = TRIBOOL_TRUE;
            }
            else if(consume->state == JSON_STATE_VALUE_STR_END) {
                consume->state = JSON_STATE_VALUE_END;
                consume->tribool = TRIBOOL_TRUE;                    
            }
            else
                consume->tribool = TRIBOOL_FALSE;
            break;
        }            
        case ']':
        {
            consume->nextTok = (void *)tok_right_bracket;

            if(consume->state == JSON_STATE_VALUE_ARRAY) {
                consume->state = JSON_STATE_VALUE_ARRAY_END;
                consume->tribool = TRIBOOL_TRUE;
            }
            else
                consume->tribool = TRIBOOL_FALSE;

            break;
        }
        default: {
            if (isalpha(c) || isdigit(c)) {
                consume->nextTok = (void *)tok_letter;

                if(consume->state == JSON_STATE_NAME_BEGIN) {
                    consume->state = JSON_STATE_NAME;
                    consume->tribool = TRIBOOL_TRUE;
                }
                else if(consume->state == JSON_STATE_VALUE_STR_BEGIN) {
                    consume->state = JSON_STATE_VALUE_STRING;
                    consume->tribool = TRIBOOL_TRUE;
                }
                else if(consume->state == JSON_STATE_VALUE_OBJECT) {
                    consume->tribool = TRIBOOL_TRUE;
                }
                else if(consume->state == JSON_STATE_VALUE_ARRAY) {
                    consume->tribool = TRIBOOL_TRUE;
                }      
                else 
                    consume->tribool = TRIBOOL_FALSE;
            }
            else {
                consume->tribool = TRIBOOL_FALSE;
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

            if(consume->state == JSON_STATE_VALUE_BEGIN) {
                consume->type = JSON_TYPE_STRING;
                consume->state = JSON_STATE_VALUE_STR_BEGIN;
                consume->tribool = TRIBOOL_TRUE;
            }

            break;
        }
        case '{':
        {
            consume->nextTok = (void *)tok_left_curly;

            if(consume->state == JSON_STATE_VALUE_BEGIN) {
                consume->state = JSON_STATE_VALUE_OBJECT_BEGIN;
                consume->type = JSON_TYPE_OBJECT;
                consume->tribool = TRIBOOL_TRUE;
            }
            else 
                consume->tribool = TRIBOOL_FALSE;

            break;
        }            
        case '[':
        {
            consume->nextTok = (void *)tok_left_bracket;

            if(consume->state == JSON_STATE_VALUE_BEGIN) {
                consume->state = JSON_STATE_VALUE_ARRAY_BEGIN;
                consume->type = JSON_TYPE_ARRAY;
                consume->tribool = TRIBOOL_TRUE;
            }
            else
                consume->tribool = TRIBOOL_FALSE;
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

            if(consume->state == JSON_STATE_VALUE_END)
            {
                consume->state = JSON_STATE_NAME_BEGIN;
                consume->tribool = TRIBOOL_TRUE;
            }
            else if(consume->state == JSON_STATE_VALUE_OBJECT)
            {
                consume->tribool = TRIBOOL_TRUE;
            }
            else if(consume->state == JSON_STATE_VALUE_ARRAY)
            {
                consume->tribool = TRIBOOL_TRUE;
            }
            else
                consume->tribool = TRIBOOL_FALSE;

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

            if(consume->state == JSON_STATE_BEGIN) {
                consume->state = JSON_STATE_NAME_BEGIN;
                consume->tribool = TRIBOOL_TRUE;
            }
            else if(consume->state == JSON_STATE_VALUE_OBJECT_BEGIN) {
                consume->state = JSON_STATE_VALUE_OBJECT;
                consume->tribool = TRIBOOL_TRUE;
            }
            else
                consume->tribool = TRIBOOL_FALSE;

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

            if(consume->state == JSON_STATE_VALUE_OBJECT_END)
            {
                consume->state = JSON_STATE_VALUE_END;
                consume->tribool = TRIBOOL_TRUE;
            }
            else
                consume->tribool = TRIBOOL_FALSE;
            break;
        }
        case '}':
        {
            consume->nextTok = (void *)tok_right_curly;
            
            if(consume->state == JSON_STATE_VALUE_OBJECT_END) {
                consume->state = JSON_STATE_VALUE_END;
                consume->tribool = TRIBOOL_TRUE;
            }
            else if(consume->state == JSON_STATE_VALUE_ARRAY_END){
                consume->state = JSON_STATE_VALUE_END;
                consume->tribool = TRIBOOL_TRUE;
            }
            else 
                    consume->tribool = TRIBOOL_FALSE;
            break;
        }
        case '\0':
        case '\r':
        case '\n': {
            consume->nextTok = (void *)tok_ctrl_letter;

            if(consume->state == JSON_STATE_VALUE_END) {
                consume->state = JSON_END;
                consume->tribool = TRIBOOL_TRUE;
            }
            else
                consume->tribool = TRIBOOL_FALSE;
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

            if(consume->state == JSON_STATE_VALUE_ARRAY_BEGIN) {
                consume->state = JSON_STATE_VALUE_ARRAY;
                consume->tribool = TRIBOOL_TRUE;
            }
            else
                consume->tribool = TRIBOOL_FALSE;
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
        case ',' :
        {
            consume->nextTok = (void *)tok_comma;

            if(consume->state == JSON_STATE_VALUE_ARRAY_END)
            {
                consume->state = JSON_STATE_VALUE_END;
                consume->tribool = TRIBOOL_TRUE;
            }
            break;
        }
        case '}':
        {
            consume->nextTok = (void *)tok_right_curly;
            if(consume->state == JSON_STATE_VALUE_ARRAY_END) {
                consume->state = JSON_STATE_VALUE_END;
                consume->tribool = TRIBOOL_TRUE;
            }
            else 
                consume->tribool = TRIBOOL_FALSE;

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

            switch (consume->state) {
                case JSON_STATE_NAME:
                {
                    consume->state = JSON_STATE_NAME_END;
                    consume->tribool = TRIBOOL_TRUE;
                    break;
                }
                case JSON_STATE_VALUE_OBJECT:
                case JSON_STATE_VALUE_ARRAY:
                {
                    consume->tribool = TRIBOOL_TRUE;
                    break;
                }
                case JSON_STATE_VALUE_STRING:
                {
                    consume->state = JSON_STATE_VALUE_STR_END;
                    consume->tribool = TRIBOOL_TRUE;
                    break;
                }
                default:
                    consume->tribool = TRIBOOL_FALSE;
                    break;
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

// CASE CR LF '\0'
JsonConsume tok_ctrl_letter(const char c, JsonConsume *objConsume)
{
    debug_message("tok_ctrl_letter\r\n");
    JsonConsume *consume = objConsume;

    switch (c) {
        case '{' :
        {
            consume->state = JSON_STATE_BEGIN;
            consume->tribool = TRIBOOL_TRUE;
            consume->nextTok = (void *)tok_left_curly;
            break;
        }
        case LF:
        case CR:
        case '\0':
        {
            consume->nextTok = (void *)tok_ctrl_letter;
            consume->tribool = TRIBOOL_INDETERMINATE;
            break;
        }
        default: {
            consume->tribool = TRIBOOL_FALSE;
            break;
        }
    }

    return *consume;
}

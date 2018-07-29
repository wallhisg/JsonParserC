#include <json/json_parser.h>
#include <json/json_parser_object.h>


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

        consume = consume_object(byte, jsonConsume);

        if(consume.tribool == TRIBOOL_TRUE)
        {
            if(jsonBuff->status != RING_STATUS_FULL)
            {
                write_one_byte_to_json_buffer(byte);
                counter++;
                
            }
            else
            {
                consume.tribool = TRIBOOL_FALSE;
                consume.type = JSON_TYPE_UNDEFINED;
                // read buffer continuously until get LF
                consume_buffer(jsonBuff, LF);
                consume_buffer(inBuff, LF);
                error("JSON BUFFER IS FULL");
                break;
            }
        }
        else if(consume.tribool == TRIBOOL_FALSE)
        {
            debug("Json string false at %d.\r\n", i);
            consume.type = JSON_TYPE_UNDEFINED;
            // read buffer continuously until get LF
            consume_buffer(jsonBuff, LF);
            consume_buffer(inBuff, LF);
            break;
        }
        // end block; end of json frame \n
        if(byte == LF)
        {
            counter++;
            consume.counter = counter;
            break;
        }
    }
    return consume;
}

JsonConsume consume_char(char c, JsonConsume *jsonConsume)
{

    JsonConsume consume;
    debug_json("tokCounter %u\r\n", jsonConsume->counter);
    consume = jsonConsume->nextTok(c, jsonConsume);
    debug_json("consume_char: %d : >> \r\n", c);
    return consume;
}

JsonConsume tok_letter_start(const char c, JsonConsume *jsonConsume)
{
    debug_json("tok_letter_start");

    JsonConsume *consume = jsonConsume;

    switch (c)
    {
        case '\"': {
            consume->nextTok = (void *)tok_dq_mark;
            consume->tribool = TRIBOOL_INDETERMINATE;
            consume->type = JSON_TYPE_STRING;
            break;
        }
        case '{': {
            consume->nextTok = (void *)tok_l_curly;
            consume->tribool = TRIBOOL_INDETERMINATE;
            consume->type = JSON_TYPE_OBJECT;
            consume->counter++;
            break;
        }
        default:
        {
            if ((c == LF) || (c == CR) || (c == '\0'))
            {
                consume->tribool = TRIBOOL_INDETERMINATE;
                consume->type = JSON_TYPE_UNDEFINED;
                consume->nextTok = (void *)tok_letter_start;
            }
            break;
        }
    }

    return *consume;
}

// "
JsonConsume tok_dq_mark(const char c, JsonConsume *jsonConsume)
{
    debug_json("tok_dq_mark");

    JsonConsume *consume = jsonConsume;

    switch (c)
    {
        case ':': {
            consume->nextTok = (void *)tok_colon;
            consume->tribool = TRIBOOL_TRUE;
            break;
        }
        case ',': {
            consume->nextTok = (void *)tok_comma;
            consume->tribool = TRIBOOL_TRUE;
            break;
        }
        case '}': {
            consume->nextTok = (void *)tok_r_curly;
            consume->counter--;

            if(consume->isObjects)
            {
                consume->tribool = TRIBOOL_TRUE;
                consume->isObjects = false;
            }
            else
                consume->tribool = TRIBOOL_INDETERMINATE;
            break;
        }
        case ']': {
            consume->nextTok = (void *)tok_r_bracket;
            consume->isObjects = false;
            consume->tribool = TRIBOOL_TRUE;
            consume->counter--;
            break;
        }
        default: {
            if (isalpha(c) || isdigit(c))
            {
                consume->nextTok = (void *)tok_letter;
                consume->tribool = TRIBOOL_TRUE;
            }
            else if ((c == '\0') || (c == '\r') || (c == '\n'))
            {
                consume->tribool = TRIBOOL_TRUE;
            }
            else
            {
                consume->tribool = TRIBOOL_FALSE;
            }
            break;
        }
    }
    return *consume;
}

// {
JsonConsume tok_l_curly(const char c, JsonConsume *jsonConsume)
{
    debug_json("tok_l_curly");

    JsonConsume *consume = jsonConsume;
    switch (c)
    {
        case  '\"': // {“
        {
            consume->nextTok = (void *)tok_dq_mark;
            consume->tribool = TRIBOOL_TRUE;
            break;
        }
        case ' ':   // space
        {
            consume->tribool = TRIBOOL_INDETERMINATE;
            break;
        }
        default:
            consume->tribool = TRIBOOL_FALSE;
            break;
    }

    return *consume;
}

// }
JsonConsume tok_r_curly(const char c, JsonConsume *jsonConsume)
{
    debug_json("tok_r_curly");

    JsonConsume *consume = jsonConsume;

    switch (c)
    {
        case ',': {
            consume->nextTok = (void *)tok_comma;
            consume->tribool = TRIBOOL_TRUE;
            break;
        }
        case '\0':
        case '\r':
        case '\n':
        {
            consume->tribool = TRIBOOL_TRUE;
            break;
        }
        case '}': {
            consume->nextTok = (void *)tok_r_curly;
            consume->counter--;

            if (consume->counter > 1)
            {
                consume->tribool = TRIBOOL_TRUE;
            }
            else
            {
                consume->tribool = TRIBOOL_INDETERMINATE;
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

//  [
JsonConsume tok_l_bracket(const char c, JsonConsume *jsonConsume)
{
    debug_json("tok_l_bracket");

    JsonConsume *consume = jsonConsume;
    // pending
    switch (c)
    {
        case '\"': {
            consume->nextTok = (void *)tok_dq_mark;
            consume->isObjects = true;
            consume->tribool = TRIBOOL_TRUE;
            break;
        }
        default:
            consume->tribool = TRIBOOL_FALSE;
            break;
    }

    return *consume;
}

//  ]
JsonConsume tok_r_bracket(const char c, JsonConsume *jsonConsume)
{
    debug_json("tok_r_bracket");

    JsonConsume *consume = jsonConsume;

    switch (c)
    {
        case '}' : {
            consume->nextTok = (void *)tok_r_curly;
            consume->tribool = TRIBOOL_INDETERMINATE;
            consume->isObjects = false;
            break;
        }
        case ',' : {
            consume->nextTok = (void *)tok_comma;
            consume->tribool = TRIBOOL_TRUE;
            break;
        }
        case '\0':
        case '\r':
        case '\n':
        {
            consume->tribool = TRIBOOL_TRUE;
            break;
        }
        default:
            consume->tribool = TRIBOOL_FALSE;
            break;
    }

    return *consume;
}

// :
JsonConsume tok_colon(const char c, JsonConsume *jsonConsume)
{
    debug_json("tok_colon");

    JsonConsume *consume = jsonConsume;
    switch (c)
    {
        case  '\"': {
            consume->nextTok = (void *)tok_dq_mark;
            consume->tribool = TRIBOOL_TRUE;
            break;
        }
        case '{': {
            consume->isObjects = true;
            consume->nextTok = (void *)tok_l_curly;
            consume->tribool = TRIBOOL_TRUE;
            consume->counter++;
            break;
        }
        case '[': {
            consume->nextTok = (void *)tok_l_bracket;
            consume->tribool = TRIBOOL_TRUE;
            consume->type = JSON_TYPE_ARRAY;
            consume->isObjects = true;
            consume->counter++;
            break;
        }
        default:
            consume->tribool = TRIBOOL_FALSE;
            break;
    }

    return *consume;
}

//  ,
JsonConsume tok_comma(const char c, JsonConsume *jsonConsume)
{
    debug_json("tok_comma");

    JsonConsume *consume = jsonConsume;

    switch (c)
    {
        case  '\"': {
            consume->nextTok = (void *)tok_dq_mark;
            consume->tribool = TRIBOOL_TRUE;
            break;
        }
        default: {
            consume->tribool = TRIBOOL_FALSE;
            break;
        }
    }

    return *consume;
}

//  0 -> 9; a -> Z
JsonConsume tok_letter(const char c, JsonConsume *jsonConsume)
{
    debug_json("tok_letter");

    JsonConsume *consume = jsonConsume;

    switch (c)
    {
        case '\"': {
            if (consume->type == JSON_TYPE_STRING)
            {
                consume->nextTok = (void *)tok_dq_mark;
                consume->tribool = TRIBOOL_INDETERMINATE;
            }
            else if (consume->type == JSON_TYPE_OBJECT)
            {
                consume->nextTok = (void *)tok_dq_mark;
                consume->tribool = TRIBOOL_TRUE;
            }
            else if (consume->type == JSON_TYPE_ARRAY)
            {
                consume->nextTok = (void *)tok_dq_mark;
                consume->tribool = TRIBOOL_TRUE;
            }
            break;
        }
        default: {
            if(isalpha(c) || isdigit(c))
            {
                consume->nextTok = (void *)tok_letter;
                consume->tribool = TRIBOOL_TRUE;
            }
            else
            {
                consume->tribool = TRIBOOL_FALSE;
            }
            break;
        }
    }

    return *consume;
}

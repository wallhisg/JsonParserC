#include <json/json_compress.h>
#include <driver/buffer_alloc.h>

void coder_block(void *compressAdd, void *codedAdd);
void decoder_block(void *decompressAdd, void *compressedAdd);

char get_ascii_coder(char c);
char get_ascii_decoder(char c);

char *json_compress_ascii(char *rawStr)
{
    // coder
    int len = strlen(rawStr);
    char *codedStr = (char*)buffer_malloc(len * sizeof(char));

    int i = 0, j = 0;
    for(i = 0; i < len; ++i)
    {
        codedStr[i] = get_ascii_coder(rawStr[i]);
    }
    // Compress ascii code
    int compressSize = len * 3 / 4 + 2;
    char *asciiCom= (char*)buffer_malloc(compressSize * sizeof(char));

    for(i = 0, j = 0; i < len; )
    {
        coder_block(&asciiCom[j], &codedStr[i]);
        i = i + 4;
        j = j + 3;
    }

    buffer_free(codedStr);

    return asciiCom;
}

char *json_decompress_ascii(char *codedStr)
{
    // decompress ascii code
    int len = strlen(codedStr);
    int lenAlign = len * 4 / 3 + 1;
    char *asciiDecom = (char*)buffer_malloc(lenAlign * sizeof(char));
    memset(asciiDecom, 0, lenAlign);

    int i = 0, j = 0;
    for(i = 0, j = 0; i < len;)
    {
        decoder_block(&asciiDecom[j], &codedStr[i]);
        i = i + 3;
        j = j + 4;
    }
    char *strDecoded = (char*)buffer_malloc(len * sizeof(char));
    memset(strDecoded, 0, len);

    len = strlen(asciiDecom);
    for(i = 0; i < len; ++i)
    {
        strDecoded[i] = get_ascii_decoder(asciiDecom[i]);
    }

    buffer_free(asciiDecom);

    return strDecoded;
}

void coder_block(void *compressAdd, void *codedAdd)
{
    CodeBlock *des = compressAdd;
    CodeBlock *src = codedAdd;

    // byte 3
    des->byte3 = src->byte3 << 2;
    des->byte3 |= src->byte2 >> 4;
    // byte 2
    des->byte2 = src->byte2 << 4;
    des->byte2 |= src->byte1 >> 2;
    // byte 1
    des->byte1 = src->byte1 << 6;
    des->byte1 |= src->byte0;
}

void decoder_block(void *decompressAdd, void *compressedAdd)
{
    CodeBlock *des = decompressAdd;
    CodeBlock *src = compressedAdd;

    // byte 0
    des->byte0 = src->byte1 & 0b00111111;
    // byte 1
    des->byte1 = src->byte1 >> 6;
    des->byte1 &= 0b00000011;
    des->byte1 |= src->byte2 << 2;
    des->byte1 &= 0b00111111;
    // byte 2
    des->byte2 = src->byte2 >> 4;
    des->byte2 &= 0b00001111;
    des->byte2 |= src->byte3 << 4;
    des->byte2 &= 0b00111111;
    // byte 3
    des->byte3 = src->byte3 >> 2;
    des->byte3 &= 0b00111111;
}

char get_ascii_coder(char c)
{
    uint8_t result;

    switch(c)
    {
        // End string
        case '\0':  /*result = '\0';  break;  // NULL*/
        case '\n':  /*result = 10;    break;  // LF*/
        case '\r':  /*result = 13;    break;  // CR*/
        result = 10;    break;  // LF
        // Token
        case '\"':  result = 4;     break;      case ',':   result = 5;     break;
        case ':':   result = 6;     break;      case ';':   result = 7;     break;
        case '[':   result = 8;     break;      case ']':   result = 9;     break;
        case '{':   result = 1;     break;      case '}':   result = 11;    break;
        // Number
        case '0':   result = 12;    break;      case '1':   result = 2;     break;
        case '2':   result = 14;    break;      case '3':   result = 15;    break;
        case '4':   result = 16;    break;      case '5':   result = 17;    break;
        case '6':   result = 18;    break;      case '7':   result = 19;    break;
        case '8':   result = 20;    break;      case '9':   result = 21;    break;
        // Letter
        case 'a':   result = 22;    break;      case 'b':   result = 23;    break;
        case 'c':   result = 24;    break;      case 'd':   result = 25;    break;
        case 'e':   result = 26;    break;      case 'f':   result = 27;    break;
        case 'g':   result = 28;    break;      case 'h':   result = 29;    break;
        case 'i':   result = 30;    break;      case 'j':   result = 31;    break;
        case 'k':   result = 32;    break;      case 'l':   result = 33;    break;
        case 'm':   result = 34;    break;      case 'n':   result = 35;    break;
        case 'o':   result = 36;    break;      case 'p':   result = 37;    break;
        case 'q':   result = 38;    break;      case 'r':   result = 39;    break;
        case 's':   result = 40;    break;      case 't':   result = 41;    break;
        case 'u':   result = 42;    break;      case 'v':   result = 43;    break;
        case 'w':   result = 44;    break;      case 'x':   result = 45;    break;
        case 'y':   result = 46;    break;      case 'z':   result = 47;    break;
        default:
            result = '\0';
            break;
    }

    return result;
}

char get_ascii_decoder(char c)
{
    char result;

    switch(c)
    {
        // End string
        case '\0':  /*result = '\0';  break;  // NULL*/
        case 10:    /*result = '\n';  break;  // LF*/
        case 13:    /*result = '\r';  break;  // CR*/
        result = '\0';  break;  // NULL
        // Token
        case 4:     result = '\"';  break;      case 5:     result = ',';   break;
        case 6:     result = ':';   break;      case 7:     result = ';';   break;
        case 8:     result = '[';   break;      case 9:     result = ']';   break;
        case 1:     result = '{';   break;      case 11:    result = '}';   break;
        // Number
        case 12:    result = '0';   break;      case 2:     result = '1';   break;
        case 14:    result = '2';   break;      case 15:    result = '3';   break;
        case 16:    result = '4';   break;      case 17:    result = '5';   break;
        case 18:    result = '6';   break;      case 19:    result = '7';   break;
        case 20:    result = '8';   break;      case 21:    result = '9';   break;
        // Letter
        case 22:    result = 'a';   break;      case 23:    result = 'b';   break;
        case 24:    result = 'c';   break;      case 25:    result = 'd';   break;
        case 26:    result = 'e';   break;      case 27:    result = 'f';   break;
        case 28:    result = 'g';   break;      case 29:    result = 'h';   break;
        case 30:    result = 'i';   break;      case 31:    result = 'j';   break;
        case 32:    result = 'k';   break;      case 33:    result = 'l';   break;
        case 34:    result = 'm';   break;      case 35:    result = 'n';   break;
        case 36:    result = 'o';   break;      case 37:    result = 'p';   break;
        case 38:    result = 'q';   break;      case 39:    result = 'r';   break;
        case 40:    result = 's';   break;      case 41:    result = 't';   break;
        case 42:    result = 'u';   break;      case 43:    result = 'v';   break;
        case 44:    result = 'w';   break;      case 45:    result = 'x';   break;
        case 46:    result = 'y';   break;      case 47:    result = 'z';   break;
        default:
            result = '\0';
            break;
    }

    return result;
}

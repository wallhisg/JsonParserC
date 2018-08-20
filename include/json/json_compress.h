#ifndef JSON_COMPRESSION_H
#define JSON_COMPRESSION_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    char byte3;
    char byte2;
    char byte1;
    char byte0;
} CodeBlock;

char *json_compress_ascii(char *rawStr);
char *json_decompress_ascii(char *codedStr);

#endif  //  JSON_COMPRESSION_H

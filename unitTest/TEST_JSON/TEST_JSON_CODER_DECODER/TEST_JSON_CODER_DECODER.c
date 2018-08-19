#include <string.h>
#include <system/system.h>
#include <stdlib.h>
#include <driver/buffer_alloc.h>
#include <json/json_compress.h>

extern void coder_block(void *compressAdd, void *codedAdd);
extern void decoder_block(void *decompressAdd, void *compressedAdd);
extern char get_ascii_coder(char c);
extern char get_ascii_decoder(char c);

char *TEST_CODER_BLOCK(char *inBuff)
{
    printf("----- TEST_CODER_BLOCK -----\r\n");
    int len = strlen(inBuff);
    int lenAlign = len;

    if(len % MEMORY_ALIGN_MULTIPLE) {
        lenAlign -= len % MEMORY_ALIGN_MULTIPLE;
        lenAlign += MEMORY_ALIGN_MULTIPLE;
    }

    char *strCoded = (char*)buffer_malloc(len * sizeof(char));

    // coder
    len = strlen(inBuff);
    int i = 0, j = 0;
    for(i = 0; i < len; ++i)
    {
        strCoded[i] = get_ascii_coder(inBuff[i]);
    }

    // Compress ascii code
    char *asciiCom= (char*)buffer_malloc(len * sizeof(char));
    for(i = 0, j = 0; i < len; )
    {
        coder_block(&asciiCom[j], &strCoded[i]);
        i = i + 4;
        j = j + 3;

    }
    buffer_free(strCoded);
    return asciiCom;
}

char *TEST_DECODER_BLOCK(char *inBuff)
{
    printf("-----TEST_DECODER_BLOCK -----\r\n");
    // decompress ascii code
    int len = strlen(inBuff);
    int lenAlign = len;

    lenAlign = lenAlign * 4 / 3;
    if(lenAlign % MEMORY_ALIGN_MULTIPLE) {
        lenAlign -= lenAlign % MEMORY_ALIGN_MULTIPLE;
        lenAlign += MEMORY_ALIGN_MULTIPLE;
    }

    char *asciiDecom = (char*)buffer_malloc(lenAlign * sizeof(char));
    memset(asciiDecom, 0, lenAlign);

    int i = 0, j = 0;
    for(i = 0, j = 0; i < len;)
    {
        decoder_block(&asciiDecom[j], &inBuff[i]);
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

int main()
{
    system_init();
    char *src = "123\n";
    int len = strlen(src);


    char *coded = TEST_CODER_BLOCK(src);
    char *decoded = TEST_DECODER_BLOCK(coded);

    printf("CODED: %s\r\n", coded);
    printf("DECODED: %s\r\n", decoded);

    printf("LENGTH OF RAW STRING: %d\r\n", len);
    printf("LENGTH OF DECOMPRESS STRING: %lu\r\n", strlen(decoded));

    buffer_free(coded);
    buffer_free(decoded);

    display_heap();

    return 0;
}

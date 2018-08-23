#include <driver/buffer_alloc.h>
#include <system/system.h>

#define HEAP_BUFFER_CONFIG_LENGTH   1024

static char HEAP[HEAP_BUFFER_CONFIG_LENGTH];
static memory_header *heap = NULL;

static void *buffer_alloc(memory_header *header, size_t size);
static void buffer_realloc(void *ptr);
static size_t verify_header(memory_header *header);
static void display_header(const memory_header *header);
void buffer_refresh();

void *buffer_malloc(const size_t size)
{
    memory_header *header = heap;
    memory_header *cursor = NULL;
    void *ret = NULL;

    size_t len = size + 1;
    if(len % MEMORY_ALIGN_MULTIPLE)
    {
            len -= len % MEMORY_ALIGN_MULTIPLE;
            len += MEMORY_ALIGN_MULTIPLE;
    }
    len += sizeof(memory_header);

    if(header == NULL)
        buffer_alloc_init();

    cursor = heap;
    while(cursor != NULL)
    {
        if(cursor->size > len && (cursor->alloc == 0))
        {
            cursor->alloc = 1;
            cursor->size = len;
            ret = buffer_alloc(cursor, len);
            break;
        }
        cursor = cursor->next;
    }
    len -= sizeof(memory_header);

    if(ret != NULL)
        memset(ret, 0, len);

    return ret;
}

static void *buffer_alloc(memory_header *ptr, size_t size)
{
    memory_header *header = ptr;
    memory_header *cursor = NULL;
    void *ret = NULL;

    cursor = header->next;
    if(cursor == NULL)
    {
        cursor = (memory_header *)((void *)header + header->size);
        cursor->alloc = 0;

        size_t pOffset = (void *)header - (void *)heap;
        cursor->size = HEAP_BUFFER_CONFIG_LENGTH - header->size - pOffset;
        cursor->next = NULL;
    }

    header->next = cursor;
    cursor->prev = header;

    ret = (void *)header + sizeof(memory_header);

    return ret;
}

void buffer_free(void* ptr)
{
    if(ptr != NULL)
    {
        void *p = ptr;
        p -= sizeof(memory_header);

        memory_header *header = (memory_header *)p;
        header->alloc = 0;

        if(verify_header(header) != 0)
        {
            printf("buffer_free unlocated\r\n");
        }
        else
        {
            buffer_realloc(header);
        }
    }
}

void buffer_refresh()
{
    memory_header *header = heap;
    while (header != NULL) {
        buffer_realloc(header);
        header = header->next;
    }
}

void buffer_realloc(void* ptr)
{
    memory_header *header = ptr;
    memory_header *old = header;
    memory_header *prev = header->prev;
    memory_header *next = header->next;

    // Regroup with block before
    if((prev != NULL) && (prev->alloc == 0))
    {
        prev->size += header->size;

        if(header->next != NULL)
        {
            prev->next = header->next;
            header->next->prev = prev;
            header = prev;
        }

        memset(old, 0, sizeof(memory_header));
    }

    // Regroup with block after
    if((next != NULL) && (next->alloc == 0))
    {
        header->size += next->size;

        if(next->next != NULL)
        {
            header->next = next->next;
            next->next->prev = header;
        }
        else
        {
            next->prev = header;
            header->next = NULL;
        }
        memset(next, 0, sizeof(memory_header));
    }
}

#define VERIFY_DEBUG    1
size_t verify_header(memory_header* header)
{
    size_t result = 0;

   if(header == NULL)
       result = 1;
   
#if VERIFY_DEBUG
    if(result)
    {
        printf("header == NULL\r\n");
    }
#endif

   if(header->alloc > 1)
        result = 1;
   
#if VERIFY_DEBUG
    if(result)
    {
        printf("header->alloc > 1\r\n");
    }
#endif

   if(header->prev == header->next)
       result = 1;
   
#if VERIFY_DEBUG
    if(result)
    {
        printf("header->prev == header->next\r\n");
    }
#endif

   if((void *)header < (void *)HEAP)
       result = 1;
   
#if VERIFY_DEBUG
    if(result)
    {
        printf("(void *)header < (void *)HEAP\r\n");
    }
#endif

   if((void *)header > (void *)HEAP + HEAP_BUFFER_CONFIG_LENGTH)
        result = 1;
   
#if VERIFY_DEBUG
    if(result)
    {
        printf("(void *)header > (void *)HEAP + HEAP_BUFFER_CONFIG_LENGTH\r\n");
    }
#endif

    return result;
}

void buffer_alloc_init()
{
    memset(HEAP, 0, HEAP_BUFFER_CONFIG_LENGTH);
    heap = (memory_header *)HEAP;
    heap->alloc = 0;
    heap->size = HEAP_BUFFER_CONFIG_LENGTH - sizeof(memory_header);
    heap->prev = NULL;
    heap->next = NULL;
}

void display_heap()
{
    printf("HEAP\r\n");
    memory_header *cursor = (memory_header *)HEAP;

    while(cursor != NULL)
    {
        display_header(cursor);
        cursor = cursor->next;
    }
}

static void display_header(const memory_header* header)
{
#if HEAP_DEBUG
    printf("\tADDR: %p  \tPREV: %p  \tNEXT: %p",
           header,
           header->prev,
           header->next);
    printf("\tALLOC: %lu  \tDATA_SIZE: %lu  \tSIZE: %lu",
           header->alloc,
           header->size - sizeof(memory_header),
           header->size);
    printf("\r\n");
#endif
}


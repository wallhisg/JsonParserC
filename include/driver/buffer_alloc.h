#ifndef BUFFER_ALLOC_H
#define BUFFER_ALLOC_H

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define HEAP_DEBUG                1
#define MEMORY_ALIGN_MULTIPLE       4

#define __SIZE_TYPE__ long unsigned int
typedef __SIZE_TYPE__ size_t;

typedef struct _memory_header {
    struct _memory_header *prev;
    size_t alloc;
    size_t size;
    struct _memory_header *next;
} memory_header;

void *buffer_malloc(const size_t size);
void buffer_free(void *ptr);

void buffer_alloc_init();
void display_heap();


#endif // BUFFER_ALLOC_H


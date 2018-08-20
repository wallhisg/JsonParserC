#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "config.h"
#include "debug.h"
#include "error.h"

#define __SIZE_TYPE__ long unsigned int
typedef __SIZE_TYPE__ size_t;

typedef enum {
    TRIBOOL_FALSE = 0,
    TRIBOOL_TRUE = 1,
    TRIBOOL_INDETERMINATE = 2
} TriBool;

void system_init();

#endif  //  SYSTEM_H

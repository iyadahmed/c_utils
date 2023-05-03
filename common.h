#pragma once

#include <stdio.h>
#include <stdlib.h>

#define STRUCT(struct_name)                 \
    typedef struct struct_name struct_name; \
    struct struct_name

// Not a good pattern to print and error and exit like that but let's do it for now
void bail(const char *msg)
{
    puts(msg);
    exit(1);
}

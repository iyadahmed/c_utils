#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "common.h"

STRUCT(linked_list_item_t)
{
    void *data;
    linked_list_item_t *prev;
};

// Append function for any structure that has a "prev" pointer
#define APPEND_TO_LINKED_LIST(last_item_ptr, new_item_ptr) \
    {                                                      \
        if ((*(last_item_ptr)) == NULL)                    \
        {                                                  \
            (new_item_ptr)->prev = NULL;                   \
        }                                                  \
        else                                               \
        {                                                  \
            (new_item_ptr)->prev = *(last_item_ptr);       \
        }                                                  \
        *(last_item_ptr) = (new_item_ptr);                 \
    }

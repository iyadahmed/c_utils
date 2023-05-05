#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "common.h"

STRUCT(linked_list_item_t)
{
    void* data;
    linked_list_item_t* prev;
};

STRUCT(linked_list_t) { linked_list_item_t* last; };

linked_list_t create_list()
{
    return (linked_list_t) { NULL };
}

void append_to_linked_list(linked_list_t* list, linked_list_item_t* new_item)
{
    new_item->prev = list->last;
    list->last = new_item;
}

#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "common.h"

STRUCT(linked_list_item_t)
{
    void* value;
    linked_list_item_t* prev;
};

STRUCT(linked_list_t) { linked_list_item_t* last; };

linked_list_t create_list()
{
    return (linked_list_t) { NULL };
}

void append_to_linked_list(linked_list_t* list, void* value)
{
    linked_list_item_t* new_item = malloc(sizeof(linked_list_item_t));
    new_item->value = value;
    new_item->prev = list->last;
    list->last = new_item;
}

void free_list(linked_list_t* list)
{
    linked_list_item_t* iter = list->last;
    while (iter != NULL) {
        linked_list_item_t* prev = iter->prev;
        free(iter);
        iter = prev;
    }
}
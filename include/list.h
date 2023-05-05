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

void append_to_linked_list(linked_list_t* list, void* value)
{
    linked_list_item_t* new_item = malloc(sizeof(linked_list_item_t));
    new_item->data = value;
    new_item->prev = list->last;
    list->last = new_item;
}

linked_list_item_t* foreach_in_list(linked_list_t* list, void (*func)(linked_list_item_t*))
{
    linked_list_item_t* iter = list->last;
    while (iter != NULL) {
        linked_list_item_t* prev = iter->prev;
        func(iter);
        iter = prev;
    }
}

void free_list_item_and_its_data(linked_list_item_t* item)
{
    free(item->data);
    free(item);
}

void free_list(linked_list_t* list)
{
    foreach_in_list(list, free);
}

void free_list_and_its_data(linked_list_t* list)
{
    foreach_in_list(list, free_list_item_and_its_data);
}
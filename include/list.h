#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "common.h"

STRUCT(list_item_t)
{
    void* data;
    list_item_t* prev;
};

STRUCT(list_t) { list_item_t* last; };

list_t create_list()
{
    return (list_t) { NULL };
}

void append_to_linked_list(list_t* list, void* value)
{
    list_item_t* new_item = malloc(sizeof(list_item_t));
    new_item->data = value;
    new_item->prev = list->last;
    list->last = new_item;
}

list_item_t* foreach_in_list(list_t* list, void (*func)(list_item_t*))
{
    list_item_t* iter = list->last;
    while (iter != NULL) {
        list_item_t* prev = iter->prev;
        func(iter);
        iter = prev;
    }
}

void free_list_item_and_its_data(list_item_t* item)
{
    free(item->data);
    free(item);
}

void free_list(list_t* list)
{
    foreach_in_list(list, free);
}

void free_list_and_its_data(list_t* list)
{
    foreach_in_list(list, free_list_item_and_its_data);
}
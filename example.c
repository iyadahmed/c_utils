#ifdef _MSC_VER
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
#endif

#include <stdio.h>

#include "list.h"
#include "map.h"
#include "utf8.h"

int main(int argc, char** argv)
{
    linked_list_t list = create_list();
    map_t map = create_map(100);

    for (int i = 0; i < 100; i++) {
        linked_list_item_t* new_item = malloc(sizeof(linked_list_item_t));
        int* data = malloc(sizeof(int));
        *data = i;
        new_item->data = data;

        append_to_linked_list(&list, new_item);

        int* key = malloc(sizeof(int));
        *key = i;

        int* value = malloc(sizeof(int));
        *value = 2 * i - 1;

        insert_to_map(&map, key, sizeof(int), value);
    }

    {
        int* value = malloc(sizeof(int));
        *value = 1337;
        INSERT_TO_MAP_BY_STRING_LITERAL_KEY(&map, "IYAD", value);
    }

    linked_list_item_t* last = list.last;
    while (last != NULL) {
        int* key = last->data;
        int* value = get_from_map(&map, key, sizeof(int));
        printf("%d %d\n", *key, *value);
        last = last->prev;
    }

    int* value = GET_FROM_MAP_BY_STRING_LITERAL_KEY(&map, "IYAD");
    printf("%d\n", *value);

    free_map(&map);

#ifdef _MSC_VER
    _CrtDumpMemoryLeaks();
#endif

    return 0;
}

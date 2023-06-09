#ifdef _MSC_VER
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "map.h"
#include "utf8.h"

static map_t* gmap;

static void print_list_item_data(list_item_t* item)
{
    int* key = item->data;
    int value = get_value_for_key(gmap, key, sizeof(int));
    printf("%d %d\n", *key, value);
}

int main(int argc, char** argv)
{
    list_t list = create_list();
    map_t map = create_map(100);
    gmap = &map;

    for (int i = 0; i < 100; i++) {

        int* key = malloc(sizeof(int));
        *key = i;

        append_to_linked_list(&list, key);

        set_value_for_key(&map, key, sizeof(int), (void*)(2 * i - 1));
    }

    set_value_for_string_key(&map, "IYAD", (void*)1337);

    foreach_in_list(&list, print_list_item_data);

    int value = get_value_for_string_key(&map, "IYAD");
    printf("%d\n", value);

    printf("%d\n", (int)get_or_set_value_for_string_key(&map, "FOO", (void*)12345));
    printf("%d\n", (int)get_or_set_value_for_string_key(&map, "FOO", (void*)99999));

    free_map(&map);
    free_list_and_its_data(&list);

#ifdef _MSC_VER
    _CrtDumpMemoryLeaks();
#endif

    return 0;
}

#ifdef _MSC_VER
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include <stdio.h>

#include "utf8.h"
#include "map.h"
#include "list.h"

int main(int argc, char** argv)
{
	linked_list_item_t* last = NULL;

	map_t set = create_map(100);

	for (int i = 0; i < 100; i++)
	{
		linked_list_item_t* new_item = malloc(sizeof(linked_list_item_t));
		int* data = malloc(sizeof(int));
		*data = i;
		new_item->data = data;
		APPEND_TO_LINKED_LIST(&last, new_item);

		int* key = malloc(sizeof(int));
		*key = i;

		int* value = malloc(sizeof(int));
		*value = 2 * i - 1;

		insert_to_map(&set, key, sizeof(int), value);
	}

	int foo = 1337;
	INSERT_TO_MAP_BY_STRING_LITERAL_KEY(&set, "IYAD", &foo);

	while (last != NULL)
	{
		int* key = last->data;
		int* value = get_from_map(&set, key, sizeof(int));
		printf("%d %d\n", *key, *value);
		last = last->prev;
	}

	int* value = GET_FROM_MAP_BY_STRING_LITERAL_KEY(&set, "IYAD");
	printf("%d\n", *value);

	free_map(&set);

#ifdef _MSC_VER
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}

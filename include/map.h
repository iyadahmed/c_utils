#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "common.h"
#include "list.h"
#include "siphash.h"

// Arbitrary chosen key
const uint8_t SIPHASH_KEY[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

STRUCT(map_bucket_item_t)
{
    uint64_t key_hash;
    void* key;
    size_t key_length;
    void* value;
};

STRUCT(map_t)
{
    linked_list_t* buckets;
    size_t num_buckets;
};

map_t create_map(size_t num_buckets)
{
    if (num_buckets == 0) {
        bail("Does not make sense to create a map with 0 buckets");
    }
    linked_list_t* buckets = calloc(num_buckets, sizeof(linked_list_t));
    if (buckets == NULL) {
        bail("Failed to allocate buckets");
    }

    return (map_t) { buckets, num_buckets };
}

void insert_to_map(map_t* map, void* key, size_t key_length, void* value)
{
    uint64_t key_hash;
    siphash(key, key_length, SIPHASH_KEY, (uint8_t*)(&key_hash), sizeof(uint64_t));
    size_t bucket_index = key_hash % (map->num_buckets);

    map_bucket_item_t* new_bucket_item = malloc(sizeof(map_bucket_item_t));
    new_bucket_item->key = key;
    new_bucket_item->key_hash = key_hash;
    new_bucket_item->key_length = key_length;
    new_bucket_item->value = value;

    append_to_linked_list(map->buckets + bucket_index, new_bucket_item);
}

void insert_to_map_by_string_key(map_t* map, const char* string, void* value)
{
    insert_to_map(map, string, strlen(string), value);
}

bool match_key(map_bucket_item_t* item, void* key, uint64_t key_hash, size_t key_length)
{
    // Compare pointers (fast), identity implies equality
    if (item->key == key) {
        return true;
    }

    // Compare hashes (fast), if hashes are different there are no way the objects can be the same
    if (item->key_hash != key_hash) {
        return false;
    }

    // Finally compare length (fast) and actual bytes (slow)
    if (item->key_length != key_length) {
        return false;
    }
    return memcmp(item->key, key, item->key_length) == 0;
}

void* get_from_map(map_t* map, void* key, size_t key_length)
{
    uint64_t key_hash;
    siphash(key, key_length, SIPHASH_KEY, (uint8_t*)(&key_hash), sizeof(uint64_t));
    size_t bucket_index = key_hash % (map->num_buckets);

    linked_list_item_t* last = map->buckets[bucket_index].last;

    while (last != NULL) {

        map_bucket_item_t* bucket_item = last->value;
        if (match_key(bucket_item, key, key_hash, key_length)) {
            return bucket_item->value;
        }

        last = last->prev;
    }
    return NULL;
}

void* get_from_map_by_string_key(map_t* map, const char* string)
{
    return get_from_map(map, string, strlen(string));
}

void free_map(map_t* map)
{
    for (size_t i = 0; i < map->num_buckets; i++) {
        linked_list_item_t* last = map->buckets[i].last;

        while (last != NULL) {
            linked_list_item_t* prev = last->prev;
            free(last->value);
            free(last);
            last = prev;
        }
    }
    free(map->buckets);
}

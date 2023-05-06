#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
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
    list_t* buckets;
    size_t num_buckets;
};

STRUCT(bucket_item_query_result_t)
{
    list_t* bucket;
    map_bucket_item_t* bucket_item;
    uint64_t key_hash;
};

map_t create_map(size_t num_buckets)
{
    if (num_buckets == 0) {
        bail("Does not make sense to create a map with 0 buckets");
    }
    list_t* buckets = calloc(num_buckets, sizeof(list_t));
    if (buckets == NULL) {
        bail("Failed to allocate buckets");
    }

    return (map_t) { buckets, num_buckets };
}

bool compare_key(map_bucket_item_t* item, void* key, uint64_t key_hash, size_t key_length)
{
    // Compare pointers (fast), identity implies equality
    if (item->key == key) {
        return true;
    }

    // Compare hashes (fast), if hashes are different there is no way the objects can be the same
    if (item->key_hash != key_hash) {
        return false;
    }

    // Finally compare length (fast) and actual bytes (slow)
    if (item->key_length != key_length) {
        return false;
    }
    return memcmp(item->key, key, item->key_length) == 0;
}

bucket_item_query_result_t find_bucket_item(map_t* map, void* key, size_t key_length)
{
    uint64_t key_hash;
    siphash(key, key_length, SIPHASH_KEY, (uint8_t*)(&key_hash), sizeof(uint64_t));
    size_t bucket_index = key_hash % (map->num_buckets);

    list_t* bucket = map->buckets + bucket_index;
    list_item_t* last = bucket->last;

    while (last != NULL) {
        map_bucket_item_t* bucket_item = last->data;
        if (compare_key(bucket_item, key, key_hash, key_length)) {
            return (bucket_item_query_result_t) { bucket, bucket_item, key_hash };
        }
        last = last->prev;
    }
    return (bucket_item_query_result_t) { bucket, NULL, key_hash };
}

void set_value_for_key(map_t* map, void* key, size_t key_length, void* value)
{
    bucket_item_query_result_t query_result = find_bucket_item(map, key, key_length);

    if (query_result.bucket_item == NULL) {
        map_bucket_item_t* new_bucket_item = malloc(sizeof(map_bucket_item_t));
        new_bucket_item->key = key;
        new_bucket_item->key_hash = query_result.key_hash;
        new_bucket_item->key_length = key_length;
        new_bucket_item->value = value;
        append_to_linked_list(query_result.bucket, new_bucket_item);
    } else {
        query_result.bucket_item->value = value;
    }
}

void set_value_for_string_key(map_t* map, const char* string, void* value)
{
    set_value_for_key(map, (char*)string, strlen(string), value);
}

void* get_value_for_key(map_t* map, void* key, size_t key_length)
{
    bucket_item_query_result_t query_result = find_bucket_item(map, key, key_length);
    if (query_result.bucket_item == NULL) {
        return NULL;
    }
    return query_result.bucket_item->value;
}

void* get_value_for_string_key(map_t* map, const char* string)
{
    return get_value_for_key(map, (char*)string, strlen(string));
}

void* get_or_set_value_for_key(map_t* map, void* key, size_t key_length, void* value)
{
    bucket_item_query_result_t query_result = find_bucket_item(map, key, key_length);

    if (query_result.bucket_item == NULL) {
        map_bucket_item_t* new_bucket_item = malloc(sizeof(map_bucket_item_t));
        new_bucket_item->key = key;
        new_bucket_item->key_hash = query_result.key_hash;
        new_bucket_item->key_length = key_length;
        new_bucket_item->value = value;
        append_to_linked_list(query_result.bucket, new_bucket_item);
        return value;
    } else {
        return query_result.bucket_item->value;
    }
}

void* get_or_set_value_for_string_key(map_t* map, const char* string, void* value)
{
    return get_or_set_value_for_key(map, (char*)string, strlen(string), value);
}

void free_map(map_t* map)
{
    for (size_t i = 0; i < map->num_buckets; i++) {
        free_list_and_its_data(map->buckets + i);
    }
    free(map->buckets);
}

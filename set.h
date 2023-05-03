#pragma once

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "list.h"
#include "common.h"
#include "siphash.h"

// Arbitrary chosen key
const uint8_t SIPHASH_KEY[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

STRUCT(bucket_item_t)
{
    uint64_t key_hash;
    void *key;
    size_t key_length;
    void *value;
    bucket_item_t *prev;
};

STRUCT(set_t)
{
    bucket_item_t **buckets;
    size_t num_buckets;
};

set_t create_set(size_t num_buckets)
{
    if (num_buckets == 0)
    {
        bail("Does not make sense to create a set with 0 buckets");
    }
    bucket_item_t **buckets = calloc(num_buckets, sizeof(bucket_item_t *));
    if (buckets == NULL)
    {
        bail("Failed to allocate buckets");
    }

    return (set_t){buckets, num_buckets};
}

void insert_to_set(set_t *set, void *key, size_t key_length, void *value)
{
    uint64_t key_hash;
    siphash(key, key_length, SIPHASH_KEY, (uint8_t *)(&key_hash), sizeof(uint64_t));
    size_t bucket_index = key_hash % (set->num_buckets);

    bucket_item_t *new_bucket_item = malloc(sizeof(bucket_item_t));
    new_bucket_item->key = key;
    new_bucket_item->key_hash = key_hash;
    new_bucket_item->key_length = key_length;
    new_bucket_item->value = value;
    new_bucket_item->prev = NULL;
    APPEND_TO_LINKED_LIST(set->buckets + bucket_index, new_bucket_item);
}

#define INSERT_TO_SET_BY_STRING_KEY(set_ptr, const_char, value_ptr) \
    insert_to_set(set_ptr, const_char, sizeof(const_char), value_ptr);

bool match_key(bucket_item_t *item, void *key, uint64_t key_hash, size_t key_length)
{
    if (item->key == key)
    {
        return true;
    }
    else if (item->key_hash == key_hash)
    {
        return true;
    }

    if (item->key_length != key_length)
    {
        return false;
    }

    return memcmp(item->key, key, item->key_length) == 0;
}

void *get_from_set(set_t *set, void *key, size_t key_length)
{
    uint64_t key_hash;
    siphash(key, key_length, SIPHASH_KEY, (uint8_t *)(&key_hash), sizeof(uint64_t));
    size_t bucket_index = key_hash % (set->num_buckets);

    bucket_item_t *last = set->buckets[bucket_index];
    while (last != NULL)
    {
        if (match_key(last, key, key_hash, key_length))
        {
            return last->value;
        }

        last = last->prev;
    }
    return NULL;
}

#define GET_FROM_SET_BY_STRING_KEY(set_ptr, const_char) \
    get_from_set(set_ptr, const_char, sizeof(const_char));

#pragma once

typedef void (*FreeValueFunction)(void *val);
typedef void (*KeyValueFunction)(const char *const key, void *const val);

struct Hm_item
{
    char *key;
    void *value;
    struct Hm_item *next;
} typedef Hm_item;

typedef struct Hashmap
{
    Hm_item **items;
    int length;
    int capacity;
    FreeValueFunction freeValueFunction;
} Hashmap;

Hashmap *new_hashmap(int capacity, FreeValueFunction freeValueFunction);

void free_hashmap(Hashmap *const hashmap);

void *hm_put(Hashmap *const hashmap, const char *const key, void *value);

void *hm_get(const Hashmap *const hashmap, const char *const key);

int hm_remove(Hashmap *const hashmap, const char *const key);

void hm_iterate(const Hashmap *const hashmap, KeyValueFunction keyValueFunction);
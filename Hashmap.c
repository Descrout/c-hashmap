#include "Hashmap.h"
#include <stdlib.h>
#include <string.h>

static unsigned int hash(const char *key, int capacity)
{
    unsigned int hash_value = 5381;
    int c;

    while ((c = *key++))
    {
        hash_value = ((hash_value << 5) + hash_value) + c;
    }

    return hash_value % capacity;
}

Hashmap *new_hashmap(int capacity, FreeValueFunction freeValueFunction)
{
    if (capacity < 1)
        return NULL;

    Hashmap *hashmap = (Hashmap *)malloc(sizeof(Hashmap));
    if (hashmap == NULL)
        return NULL;

    hashmap->capacity = capacity;
    hashmap->length = 0;
    hashmap->freeValueFunction = freeValueFunction;

    hashmap->items = (Hm_item **)calloc(hashmap->capacity, sizeof(Hm_item *));
    if (hashmap->items == NULL)
    {
        free(hashmap);
        return NULL;
    }

    for (int i = 0; i < hashmap->capacity; i++)
    {
        hashmap->items[i] = NULL;
    }

    return hashmap;
}

static void free_item(Hm_item *const item, FreeValueFunction freeValueFunction)
{
    if (item == NULL)
        return;

    if (item->key != NULL)
        free(item->key);

    if (item->value != NULL)
        freeValueFunction(item->value);

    free_item(item->next, freeValueFunction);
    free(item);
}

void free_hashmap(Hashmap *const hashmap)
{
    if (hashmap == NULL)
        return;

    for (int i = 0; i < hashmap->capacity; i++)
    {
        Hm_item *item = hashmap->items[i];
        free_item(item, hashmap->freeValueFunction);
    }

    free(hashmap->items);
    free(hashmap);
}

void *hm_put(Hashmap *const hashmap, const char *const key, void *value)
{
    if (value == NULL)
        return NULL;
    if (hashmap == NULL)
        return NULL;
    if (hashmap->capacity == hashmap->length)
    {
        hashmap->freeValueFunction(value);
        return NULL;
    }
    if (key == NULL)
    {
        hashmap->freeValueFunction(value);
        return NULL;
    }

    Hm_item *item = (Hm_item *)malloc(sizeof(Hm_item));
    if (item == NULL)
    {
        hashmap->freeValueFunction(value);
        return NULL;
    }

    size_t len = strlen(key) + 1;
    item->key = (char *)malloc(len);
    if (item->key == NULL)
    {
        hashmap->freeValueFunction(value);
        free(item);
        return NULL;
    }
    memcpy(item->key, key, len);

    item->value = value;
    item->next = NULL;

    unsigned int index = hash(key, hashmap->capacity);

    if (hashmap->items[index] == NULL)
    {
        hashmap->items[index] = item;
        hashmap->length++;
        return value;
    }

    Hm_item *current = hashmap->items[index];
    while (1)
    {
        if (strcmp(current->key, key) == 0)
        {
            free(item->key);
            free(item);
            hashmap->freeValueFunction(current->value);
            current->value = value;
            return value;
        }
        if (current->next == NULL)
        {
            break;
        }
        current = current->next;
    }

    current->next = item;
    hashmap->length++;

    return value;
}

void *hm_get(const Hashmap *const hashmap, const char *const key)
{
    if (hashmap == NULL)
        return NULL;
    if (key == NULL)
        return NULL;

    unsigned int index = hash(key, hashmap->capacity);
    if (hashmap->items[index] == NULL)
        return NULL;

    Hm_item *current = hashmap->items[index];
    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0)
        {
            return current->value;
        }
        current = current->next;
    }

    return NULL;
}

int hm_remove(Hashmap *const hashmap, const char *const key)
{
    if (hashmap == NULL)
        return 0;
    if (key == NULL)
        return 0;

    unsigned int index = hash(key, hashmap->capacity);
    if (hashmap->items[index] == NULL)
        return 0;

    Hm_item *current = hashmap->items[index];
    Hm_item *before = NULL;

    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0)
        {
            if (before == NULL)
            {
                hashmap->items[index] = current->next;
            }
            else
            {
                before->next = current->next;
            }
            free_item(current, hashmap->freeValueFunction);
            hashmap->length--;
            return 1;
        }
        before = current;
        current = current->next;
    }

    return 0;
}

void hm_iterate(const Hashmap *const hashmap, KeyValueFunction keyValueFunction)
{
    if (hashmap == NULL)
        return;

    for (int i = 0; i < hashmap->capacity; i++)
    {
        Hm_item *item = hashmap->items[i];
        while (item != NULL)
        {
            keyValueFunction(item->key, item->value);
            item = item->next;
        }
    }
}
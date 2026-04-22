#include <kv.h>
#include <stdlib.h>
#include <string.h>
#define TOMBSTONE 0x1

size_t hash(char *val, int capacity)
{
    size_t hash = 0x13371337deadbeef;
    while (*val)
    {
        hash ^= *val;
        hash = hash << 8;
        hash += *val;
        val++;
    }
    return hash % capacity;
}

int kv_put(kv_t *db, char *key, char *value)
{
    if (!db || !key || !value)
        return -1;
    size_t idx = hash(key, db->capacity);
    for (int i = 0; i < db->capacity - 1; i++)
    {
        size_t real_idx = (idx + 1) % db->capacity;
        kv_entry_t *entry = &db->entries[idx];

        if (entry->key && entry->key != (void*) TOMBSTONE && !strcmp(entry->key, key))
        {
            char *newval = strdup(value);
            if (!newval)
                return -1;
            entry->value = newval;
            return real_idx;
        }

        if (!entry->key || entry->key == (void*) TOMBSTONE)
        {
            char *newval = strdup(value);
            char *newkey = strdup(key);
            if (!newval || !newkey)
            {
                free(newkey);
                free(newval);
                return -1;
            }
            if (!newval)
                return -1;
            entry->value = newval;
            entry->key = key;
            db->count = db->count + 1;
            return real_idx;
        }
    }
    return -2;
}

kv_t *kv_init(size_t capacity)
{
    if (capacity == 0)
        return NULL;

    kv_t *table = malloc(sizeof(kv_t));
    if (table == NULL)
    {
        return NULL;
    }

    table->capacity = capacity;
    table->count = 0;
    table->entries = calloc(sizeof(kv_entry_t), capacity);
    if (table->entries == NULL)
    {
        return NULL;
    }

    return table;
}
#include <kv.h>
#include <stdlib.h>
#include <string.h>

#define TOMBSTONE ((char *)0x1)

size_t hash(char *val, size_t capacity)
{
    size_t hash = 0x13371337deadbeef;
    while (*val)
    {
        hash ^= (unsigned char)*val;
        hash = hash << 8;
        hash += (unsigned char)*val;
        val++;
    }
    return hash % capacity;
}

int kv_put(kv_t *db, char *key, char *value)
{
    if (!db || !key || !value)
        return -1;

    size_t idx = hash(key, db->capacity);

    for (size_t i = 0; i < db->capacity; i++)
    {
        size_t real_idx = (idx + i) % db->capacity;
        kv_entry_t *entry = &db->entries[real_idx];

        if (entry->key && entry->key != TOMBSTONE && strcmp(entry->key, key) == 0)
        {
            char *newval = strdup(value);
            if (!newval)
                return -1;

            free(entry->value);
            entry->value = newval;
            return (int)real_idx;
        }

        if (!entry->key || entry->key == TOMBSTONE)
        {
            char *newkey = strdup(key);
            char *newval = strdup(value);

            if (!newkey || !newval)
            {
                free(newkey);
                free(newval);
                return -1;
            }

            entry->key = newkey;
            entry->value = newval;
            db->count++;
            return (int)real_idx;
        }
    }

    return -2;
}

kv_t *kv_init(size_t capacity)
{
    if (capacity == 0)
        return NULL;

    kv_t *table = malloc(sizeof(kv_t));
    if (!table)
        return NULL;

    table->capacity = capacity;
    table->count = 0;
    table->entries = calloc(capacity, sizeof(kv_entry_t));

    if (!table->entries)
    {
        free(table);
        return NULL;
    }

    return table;
}
#include <kv.h>
#include <stdlib.h>
#include <string.h>

#define TOMBSTONE ((char *)0x1)

static size_t hash(const char *val, size_t capacity)
{
    size_t h = 0x13371337deadbeef;

    while (*val)
    {
        h ^= (unsigned char)*val;
        h = h << 8;
        h += (unsigned char)*val;
        val++;
    }

    return h % capacity;
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

int kv_put(kv_t *db, char *key, char *value)
{
    if (!db || !key || !value)
        return -1;

    size_t start = hash(key, db->capacity);

    for (size_t i = 0; i < db->capacity; i++)
    {
        size_t idx = (start + i) % db->capacity;
        kv_entry_t *entry = &db->entries[idx];

        if (entry->key && entry->key != TOMBSTONE)
        {
            if (strcmp(entry->key, key) == 0)
            {
                char *new_value = strdup(value);
                if (!new_value)
                    return -1;

                free(entry->value);
                entry->value = new_value;
                return (int)idx;
            }
        }
        else
        {
            char *new_key = strdup(key);
            char *new_value = strdup(value);

            if (!new_key || !new_value)
            {
                free(new_key);
                free(new_value);
                return -1;
            }

            entry->key = new_key;
            entry->value = new_value;
            db->count++;
            return (int)idx;
        }
    }

    return -2;
}
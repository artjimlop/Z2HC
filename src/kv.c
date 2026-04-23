#include <kv.h>
#include <stdlib.h>
#include <string.h>

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
                return 0;
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
            return 0;
        }
    }

    return -2;
}

char *kv_get(kv_t *db, char *key)
{
    if (!db || !key)
        return NULL;

    size_t start = hash(key, db->capacity);

    for (size_t i = 0; i < db->capacity; i++)
    {
        size_t idx = (start + i) % db->capacity;
        kv_entry_t *entry = &db->entries[idx];
        if (entry->key == NULL)
        {
            return NULL;
        }
        if (entry->key &&
            entry->key != TOMBSTONE &&
            !strcmp(entry->key, key))
        {
            return entry->value;
        }
    }
    return NULL;
}

int kv_delete(kv_t *db, char *key)
{
    if (!db || !key)
        return -1;

    size_t start = hash(key, db->capacity);

    for (size_t i = 0; i < db->capacity; i++)
    {
        size_t idx = (start + i) % db->capacity;
        kv_entry_t *entry = &db->entries[idx];
        if (entry->key == NULL)
        {
            return -1;
        }
        if (entry->key &&
            entry->key != TOMBSTONE &&
            !strcmp(entry->key, key))
        {
            free(entry->key);
            free(entry->value);
            db->count--;
            entry->key = TOMBSTONE;
            entry->value = NULL;
            return idx;
        }
    }
    return -1;
}

int kv_free(kv_t *db) {
    if(!db) return -1;

    for(int i = 0; i < db->capacity; i++) {
        kv_entry_t *e = &db->entries[i];
        if(e->key && e->key != (void*) TOMBSTONE) {
            free(e->key);
            free(e->value);
            e->key = NULL;
            e->value = NULL;
            db->count--;
        }
    }
    free(db->entries);
    free(db);
    return 0;
}
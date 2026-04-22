#include <stdio.h>
#include <kv.h>

int main()
{
    kv_t *table = kv_init(3);
    printf("%p\n", table);
    kv_put(table, "hehe", "haha");
    kv_put(table, "hehe", "hoehoe");
    kv_put(table, "wewe", "hoehoe");

    for (int i = 0; i < table->capacity; i++)
    {
        if (table->entries[i].key)
        {
            printf("%s: %s \n", table->entries[i].key, table->entries[i].value);
        }
    }
}
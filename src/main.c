#include <stdio.h>
#include <stdlib.h>
#include <kv.h>

int main(void)
{
    kv_t *table = kv_init(3);
    kv_put(table, "color", "red");
    printf("%s\n", kv_get(table, "color"));
    kv_delete(table, "color");
    printf("%s\n", kv_get(table, "color"));
    kv_free(table);
}
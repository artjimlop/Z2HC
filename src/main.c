#include <stdio.h>
#include <stdlib.h>
#include <kv.h>

static int tests_run = 0;
static int tests_failed = 0;

#define ASSERT_TRUE(cond, msg)                                                  \
    do                                                                          \
    {                                                                           \
        tests_run++;                                                            \
        if (cond)                                                               \
        {                                                                       \
            printf("[PASS] %s\n", msg);                                         \
        }                                                                       \
        else                                                                    \
        {                                                                       \
            printf("[FAIL] %s\n", msg);                                         \
            tests_failed++;                                                     \
        }                                                                       \
    } while (0)

static void test_kv_put_basic_insertion(void)
{
    printf("\n=== kv_put Basic Insertion ===\n");

    kv_t *table = kv_init(3);

    ASSERT_TRUE(table != NULL, "kv_init succeeds before put tests");

    if (!table)
        return;

    ASSERT_TRUE(kv_put(table, "a", "1") == 0, "kv_put returns 0 for first key insertion");
    ASSERT_TRUE(table->count == 1, "Count is 1 after first put");

    ASSERT_TRUE(kv_put(table, "b", "2") == 0, "kv_put returns 0 for second key insertion");
    ASSERT_TRUE(table->count == 2, "Count is 2 after second put");

    ASSERT_TRUE(kv_put(table, "c", "3") == 0, "kv_put returns 0 for third key insertion");
    ASSERT_TRUE(table->count == 3, "Count is 3 after third put");
}

static void test_kv_put_null_inputs(void)
{
    printf("\n=== kv_put NULL Inputs ===\n");

    kv_t *table = kv_init(3);

    ASSERT_TRUE(table != NULL, "kv_init succeeds before null input tests");

    if (!table)
        return;

    ASSERT_TRUE(kv_put(NULL, "a", "1") == -1, "kv_put returns -1 when db is NULL");
    ASSERT_TRUE(kv_put(table, NULL, "1") == -1, "kv_put returns -1 when key is NULL");
    ASSERT_TRUE(kv_put(table, "a", NULL) == -1, "kv_put returns -1 when value is NULL");
    ASSERT_TRUE(table->count == 0, "Count remains 0 after all rejected puts");
}

static void test_kv_put_update_existing_key(void)
{
    printf("\n=== kv_put Update Existing Key ===\n");

    kv_t *table = kv_init(3);

    ASSERT_TRUE(table != NULL, "kv_init succeeds before update tests");

    if (!table)
        return;

    ASSERT_TRUE(kv_put(table, "color", "red") == 0, "Initial kv_put of 'color'='red' returns 0");
    ASSERT_TRUE(table->count == 1, "Count is 1 after initial insert");

    ASSERT_TRUE(kv_put(table, "color", "blue") == 0, "kv_put of 'color'='blue' (update) returns 0");
    ASSERT_TRUE(table->count == 1, "Count remains 1 after update (no new entry)");
}

int main(void)
{
    test_kv_put_basic_insertion();
    test_kv_put_null_inputs();
    test_kv_put_update_existing_key();

    printf("\n=== Summary ===\n");
    printf("Total tests run: %d\n", tests_run);
    printf("Passed: %d\n", tests_run - tests_failed);
    printf("Failed: %d\n", tests_failed);

    kv_t *table = kv_init(3);
    kv_put(table, "color", "red");
    printf("%s\n", kv_get(table, "color"));
    return tests_failed == 0 ? 0 : 1;
}
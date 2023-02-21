#ifndef HASHTABLE_H
#define HASHTABLE_H

#define HASH_TABLE_INIT_SIZE            100

#define HT_STATUS_OK                    0
#define HT_STATUS_KEY_NOT_FOUND_ERR     1
#define HT_STATUS_KEY_EXIST_ERR         2

struct Array;
struct Value;

typedef struct HashTableItem {
    char            *key;
    struct Value    *value;
    char            is_deleted;
} HashTableItem;

typedef struct HashTable {
    struct Array*   buf;
    struct Array*   keys;
    unsigned long   size;
} HashTable;

HashTable *hashtable_create();

int hashtable_read(HashTable *ht, struct Value *dest, const char *key);
int hashtable_insert(HashTable *ht, const char *key, void *value, unsigned long size);
int hashtable_update(HashTable *ht, const char *key, void *value, unsigned long size);
int hashtable_delete(HashTable *ht, const char *key);

struct Value *hashtable_get(const HashTable *ht, const char *key);
HashTableItem *hashtable_find(const HashTable *ht, const char *key);

HashTableItem *hashtable_item_create(const char *key, void *value, unsigned long size);
void hashtable_item_free(HashTableItem *ht);
unsigned long hashtable_calc_hash(const HashTable *ht, const char *key);

#endif // HASHTABLE_H
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"
#include "array.h"
#include "value.h"

#include "new.h"

HashTable *hashtable_create() {
    HashTable *htable = NEW(HashTable);
    htable->buf = NEW_ARRAY();
    htable->keys = NEW_ARRAY();
    htable->size = HASH_TABLE_INIT_SIZE;
    for (int i = 0; i < HASH_TABLE_INIT_SIZE; ++i) {
        Array* arr = NEW_ARRAY();
        array_append(htable->buf, (void *)arr, sizeof(Array));
    }
    return htable;
}

int hashtable_read(HashTable *ht, Value *dest, const char *key) {
     Value *value = hashtable_get(ht, key);
     if (value) {
        memcpy(dest, value, sizeof(Value));
        return HT_STATUS_OK;
     }
     return HT_STATUS_KEY_NOT_FOUND_ERR;
}

int hashtable_insert(HashTable *ht, const char *key, void *value, unsigned long size) {
    unsigned long hash = hashtable_calc_hash(ht, key);
    unsigned long key_size = sizeof(const char) * strlen(key);
    Array *items_list = (Array *)array_get(ht->buf, hash);

    HashTableItem *ht_item = hashtable_find(ht, key);
    if (ht_item != NULL) {
        return HT_STATUS_KEY_EXIST_ERR;
    } else {
        ht_item = hashtable_item_create(key, value, size);
        array_append(items_list, (void *)ht_item, sizeof(HashTableItem));
        array_append(ht->keys, (void *)key, key_size);
    }
    return HT_STATUS_OK;
}

int hashtable_update(HashTable *ht, const char *key, void *value, unsigned long size) {
    HashTableItem *ht_item = hashtable_find(ht, key); 
    if (ht_item != NULL) {
        value_delete(ht_item->value);
        ht_item->value = value_create(value, size);
    } else {
        return HT_STATUS_KEY_NOT_FOUND_ERR;
    }
    return HT_STATUS_OK;
}

int hashtable_delete(HashTable *ht, const char *key) {
    HashTableItem *ht_item = hashtable_find(ht, key); 
    if (ht_item == NULL) {
        return HT_STATUS_KEY_NOT_FOUND_ERR;
    } else {
        hashtable_item_free(ht_item);
    }
    return HT_STATUS_OK;
}

HashTableItem *hashtable_find(const HashTable *ht, const char *key) {
    unsigned long hash = hashtable_calc_hash(ht, key);
    Array *items_list = (Array *)array_get(ht->buf, hash);

    for (unsigned long i = 0; i < items_list->length; ++i) {
        HashTableItem *ht_item = (HashTableItem *)array_get(items_list, i);
        if (ht_item->is_deleted == 0) {
            if (ht_item->key != NULL) {
                if (strcmp(ht_item->key, key) == 0) {
                    return ht_item;
                }
            }
        }
    }
    return NULL;
}

Value *hashtable_get(const HashTable *ht, const char *key) {
    HashTableItem *item = hashtable_find(ht, key); 
    if (item != NULL) {
        return item->value;
    }
    return NULL;
}

HashTableItem *hashtable_item_create(const char *key, void *value, unsigned long size) {
    HashTableItem *ht_item = NEW(HashTableItem);
    ht_item->key = (char *)malloc(sizeof(char) * strlen(key));
    strcpy(ht_item->key, key);
    ht_item->value = NEW_VALUE(value, size);
    ht_item->is_deleted = 0;
    return ht_item;
}

void hashtable_item_free(HashTableItem *ht) {
    free(ht->key);
    value_delete(ht->value);
    free(ht);
    ht->is_deleted = 1;
}

unsigned long hashtable_calc_hash(const HashTable* ht, const char *key) {
    unsigned long result = 0;
    unsigned long key_len = strlen(key);

    for (unsigned long i = 0; i < key_len; ++i) {
        result = ((ht->size - 1) * result + key[i]) % ht->size;
    }
    result = (result * 2 + 1) % ht->size;
    return result;
}
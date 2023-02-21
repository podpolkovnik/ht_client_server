#ifndef ARRAY_H
#define ARRAY_H

#define ARRAY_CAP_LIMIT 10000

struct Value;

typedef struct ArrayItem {
    struct Value *value;
} ArrayItem;


typedef struct Array {
    unsigned long length;
    ArrayItem *buf_;
    unsigned long cap_;
} Array;

Array *array_create();
void array_append(Array *array, void *value, unsigned long size);
void *array_get(const Array *array, unsigned long index);

ArrayItem *array_item_create(void *value, unsigned long size);

#endif // ARRAY_H
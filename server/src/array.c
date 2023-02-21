#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "array.h"
#include "value.h"

#include "new.h"

Array *array_create() {
    Array *array = NEW(Array);
    array->buf_ = NULL;
    array->length = 0;
    array->cap_ = 1;
    return array;
}

void array_append(Array* array, void* value, unsigned long size) {
    if (array->buf_ == NULL) {
        array->buf_ = NEW(ArrayItem);
    } else {
        if (array->length + 1 > array->cap_) {
            array->cap_ *= 2;
            assert(array->cap_ <= ARRAY_CAP_LIMIT);
            array->buf_ = (ArrayItem *)realloc(array->buf_, sizeof(ArrayItem) * array->cap_);
        }
    }

    ArrayItem* new_item = array_item_create(value, size);
    memcpy((void *)&(*((ArrayItem *)array->buf_ + array->length)), (void *)new_item, sizeof(ArrayItem));
    ++array->length;
}

void *array_get(const Array *array, unsigned long index) {
    return ((ArrayItem *)array->buf_ + index)->value->data;
}

ArrayItem *array_item_create(void *value, unsigned long size) {
    ArrayItem *item = NEW(ArrayItem);
    item->value = value_create(value, size);
    return item;
}
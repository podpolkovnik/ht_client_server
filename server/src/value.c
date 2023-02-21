#include <string.h>
#include <stdlib.h>

#include "value.h"

#include "new.h"

Value *value_create(const void *data, unsigned long size) {
    Value *val = NEW(Value);
    val->data = malloc(size);
    memcpy(val->data, data, size);
    val->size = size;
    return val;
}

void value_delete(Value *value) {
    free(value->data);
    free(value);
}

#ifndef VALUE_H
#define VALUE_H

typedef struct Value {
    void *data;
    unsigned long size;
} Value;

Value *value_create(const void *data, unsigned long size);
void value_delete(Value *value);

#endif // VALUE_H
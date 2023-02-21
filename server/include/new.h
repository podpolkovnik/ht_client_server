#ifndef NEW_H
#define NEW_H

#ifndef	_STRING_H
#include <string.h>
#endif

#define NEW(type) (type *)malloc(sizeof(type))
#define NEW_CHAR(len) (char *)malloc(sizeof(char) * len)

#ifdef ARRAY_H
#define NEW_ARRAY() array_create()
#endif

#ifdef VALUE_H
#define NEW_VALUE(data, size) value_create(data, size)
#endif

#endif // NEW_H
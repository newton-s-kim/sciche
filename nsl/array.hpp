#pragma once

#include <stdlib.h>

//> Strings allocate
#define ALLOCATE(type, count) (type*)malloc(sizeof(type) * (count))
//> free

#define FREE(type, pointer) free(pointer)
//< free

//< Strings allocate
#define GROW_CAPACITY(capacity) ((capacity) < 8 ? 8 : (capacity) * 2)
//> grow-array

#define GROW_ARRAY(type, pointer, oldCount, newCount) (type*)realloc(pointer, sizeof(type) * (newCount))
//< grow-array
//> free-array

#define FREE_ARRAY(type, pointer, oldCount) free(pointer)
//< free-array

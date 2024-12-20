/*
* MIT License
*
* Copyright (c) 2024 Semih Artan
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
*/
#pragma once 

#ifndef SHJ_VECTOR_H
#define SHJ_VECTOR_H	1

#include "defs.h"
#include "array.h"
#include <errno.h>

#include <stdbool.h>
#include <stdint.h>



#define DECL_COMPFUNC_(name, type)					\
int32_t compare_##name(type l, type r);


#define DEFINE_COMPFUNC_(name, type)				\
int32_t compare_##name(type l, type r)				\
{													\
	if (l < r)										\
		return -1;									\
	else if (l > r)									\
		return 1;									\
	return 0;										\
}

#define DECL_COMPFUNC(type) DECL_COMPFUNC_(type, type)
#define DEFINE_COMPFUNC(type) DEFINE_COMPFUNC_(type, type)

DECL_COMPFUNC(char)
DECL_COMPFUNC_(unsigned_char, unsigned char)
DECL_COMPFUNC(short)
DECL_COMPFUNC_(unsigned_short, unsigned short)
DECL_COMPFUNC(int)
DECL_COMPFUNC_(unsigned_int, unsigned int)
DECL_COMPFUNC_(long_long, long long)
DECL_COMPFUNC_(unsigned_long_long, unsigned long long)

DECL_COMPFUNC(int8_t)
DECL_COMPFUNC(int16_t)
DECL_COMPFUNC(int32_t)
DECL_COMPFUNC(int64_t)
DECL_COMPFUNC(uint8_t)
DECL_COMPFUNC(uint16_t)
DECL_COMPFUNC(uint32_t)
DECL_COMPFUNC(uint64_t)

DECL_COMPFUNC(float)
DECL_COMPFUNC(double)
DECL_COMPFUNC_(long_double, long double)

#define DEFAULT_CAPACITY_SZ			8



#define SHJ_DECLARE_VECTOR(name, type)																\
extern size_t name##_capacity;																		\
typedef int (*compare_##name##_t)(type left, type right);											\
typedef struct																						\
{																									\
	size_t count;																					\
	size_t capacity;																				\
	compare_##name##_t comparer;																	\
	array_##name##_t* array;																		\
}vector_##name##_t;																					\
vector_##name##_t* vector_##name##_alloc(compare_##name##_t _comparer, ...);						\
void vector_##name##_free(vector_##name##_t* self);													\
void vector_##name##_clear(vector_##name##_t* self);												\
type vector_##name##_get(vector_##name##_t* self, size_t index);									\
type vector_##name##_set(vector_##name##_t* self, size_t index, type item);			     			\
int32_t vector_##name##_index_of(vector_##name##_t* self, type item);								\
type vector_##name##_insert(vector_##name##_t* self, size_t index, type item);						\
type vector_##name##_push_back(vector_##name##_t* self, type item);									\
type vector_##name##_push_front(vector_##name##_t* self, type item);								\
type vector_##name##_delete(vector_##name##_t* self, size_t index);									\
type vector_##name##_remove(vector_##name##_t* self, type item);									\
type vector_##name##_pop_back(vector_##name##_t* self);												\
type vector_##name##_pop_front(vector_##name##_t* self);											\
bool vector_##name##_contains(vector_##name##_t* self, type item);									\
type vector_##name##_insert_after(vector_##name##_t* self, type after, type item);					\
type vector_##name##_insert_before(vector_##name##_t* self, type before, type item);


#define SHJ_DEFINE_VECTOR(name, type)																\
extern size_t name##_capacity = DEFAULT_CAPACITY_SZ;												\
vector_##name##_t* vector_##name##_alloc(compare_##name##_t _comparer, ...)							\
{																									\
	vector_##name##_t* self = (vector_##name##_t*)NEWOBJP(self);								    \
	SHJ_RETURN_E_OUTOFMEM(self, NULL);																\
	self->count = 0;																				\
	self->comparer = _comparer;																		\
	self->capacity = name##_capacity;																\
	self->array = array_##name##_alloc(name##_capacity != DEFAULT_CAPACITY_SZ ?						\
	name##_capacity : DEFAULT_CAPACITY_SZ);															\
	name##_capacity = DEFAULT_CAPACITY_SZ;															\
	return self;																					\
}																									\
void vector_##name##_free(vector_##name##_t* self)													\
{																									\
	if (self)																						\
	{																								\
		if (self->array)																			\
			array_##name##_free(self->array);														\
		free((void*)self);																			\
	}																								\
}																									\
void vector_##name##_clear(vector_##name##_t* self)													\
{																									\
	array_##name##_clear(self->array);																\
	self->count = 0;																				\
}																									\
type vector_##name##_get(vector_##name##_t* self, size_t index)										\
{																									\
	if (index < 0 || index >= self->count)															\
		return NULL;																				\
	return array_##name##_get(self->array, index);													\
}																									\
type vector_##name##_set(vector_##name##_t* self, size_t index, type item)							\
{																									\
	if (index < 0 || index >= self->count)															\
		return NULL;																				\
	return array_##name##_set(self->array, index, item);											\
}																									\
int32_t vector_##name##_index_of(vector_##name##_t* self, type item)								\
{																									\
	for (size_t i = 0; i < self->count; i++)														\
	{																								\
		if (self->comparer(vector_##name##_get(self, i), item) == 0)								\
			return i;																				\
	}																								\
	return -1;																						\
}																									\
type vector_##name##_insert(vector_##name##_t* self, size_t index, type item)						\
{																									\
	size_t capacity = self->capacity;																\
	size_t count = self->count;																		\
	if (self->count >= capacity)																	\
	{																								\
		array_##name##_t* dest = array_##name##_alloc(capacity * 2);								\
		array_##name##_copy(dest, 0, self->array, 0, count);										\
		array_##name##_free(self->array);															\
		self->array = dest;																			\
		self->capacity = capacity * 2;																\
	}																								\
	if (index < count)																				\
	{																								\
		array_##name##_copy(self->array, index + 1, self->array, index, count - index);				\
	}																								\
	array_##name##_set(self->array, index, item);													\
	self->count++;																					\
	return item;																					\
}																									\
type vector_##name##_push_back(vector_##name##_t* self, type item)									\
{																									\
	return vector_##name##_insert(self, self->count, item);											\
}																									\
type vector_##name##_push_front(vector_##name##_t* self, type item)									\
{																									\
	return vector_##name##_insert(self, 0, item);													\
}																									\
type vector_##name##_delete(vector_##name##_t* self, size_t index)									\
{																									\
	if (index < 0 || index >= self->count)															\
		return NULL;																				\
	type deleted = vector_##name##_get(self, index);												\
	vector_##name##_set(self, index, NULL);															\
	if (index < self->count-- - 1)																	\
		array_##name##_copy(self->array, index, self->array, index + 1, self->count - index);		\
	return deleted;																					\
}																									\
type vector_##name##_remove(vector_##name##_t* self, type item)										\
{																									\
	size_t index = vector_##name##_index_of(self, item);											\
	return vector_##name##_delete(self, index);														\
}																									\
type vector_##name##_pop_back(vector_##name##_t* self)												\
{																									\
	return vector_##name##_delete(self, self->count - 1);											\
}																									\
type vector_##name##_pop_front(vector_##name##_t* self)												\
{																									\
	return vector_##name##_delete(self, 0);															\
}																									\
bool vector_##name##_contains(vector_##name##_t* self, type item)									\
{																									\
	return vector_##name##_index_of(self, item) != -1;												\
}																									\
type vector_##name##_insert_after(vector_##name##_t* self, type after, type item)					\
{																									\
	size_t index_after = vector_##name##_index_of(self, after) + 1;									\
	return vector_##name##_insert(self, index_after, item);											\
}																									\
type vector_##name##_insert_before(vector_##name##_t* self, type before, type item)					\
{																									\
	size_t index_before = vector_##name##_index_of(self, before) - 1;								\
	vector_##name##_insert(self, index_before, item);												\
} 

#endif 
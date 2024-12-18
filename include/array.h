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

#ifndef SHJ_ARRAY_H
#define SHJ_ARRAY_H		1
 
#include "defs.h"

#include <stdbool.h>
#include <stdlib.h>
#include <memory.h>


#define SHJ_DECLARE_ARRAY(name, type)													\
typedef struct																			\
{																						\
	size_t length;																		\
	type* inner;																		\
}array_##name##_t;																		\
array_##name##_t* array_##name##_alloc(size_t length);									\
void array_##name##_free(array_##name##_t* self);										\
type array_##name##_get(array_##name##_t* self, size_t index);							\
type array_##name##_set(array_##name##_t* self, size_t index, type value);				\
void array_##name##_clear_from_size(array_##name##_t* self, size_t from, size_t size);	\
void array_##name##_clear_from(array_##name##_t* self, size_t from);					\
void array_##name##_clear(array_##name##_t* self);										\
void array_##name##_copy(array_##name##_t* destination, size_t destinationIndex, array_##name##_t* source, size_t sourceIndex, size_t size);

#define SHJ_DEFINE_ARRAY(name, type)													\
																						\
array_##name##_t* array_##name##_alloc(size_t length)									\
{																						\
	array_##name##_t* self = (array_##name##_t*)NEWOBJP(self);							\
	SHJ_RETURN_E_OUTOFMEM(self, NULL);													\
	self->length = length;																\
	self->inner = (type*)NEWOBJPN(self->inner, self->length);						    \
	SHJ_RETURN_E_OUTOFMEM(self->inner, NULL);											\
	memset(self->inner, 0, self->length * sizeof(type));								\
	return self;																		\
}																						\
void array_##name##_free(array_##name##_t* self)										\
{																						\
	if (self)																			\
	{																					\
		if (self->inner)																\
			free((void*)self->inner);													\
		free((void*)self);																\
	}																					\
}																						\
type array_##name##_get(array_##name##_t* self, size_t index)							\
{																						\
	if (index < 0 || index >= self->length)												\
		return NULL;																	\
	return self->inner[index];															\
}																						\
type array_##name##_set(array_##name##_t* self, size_t index, type value)				\
{																						\
	if (index < 0 || index >= self->length)												\
		return;																			\
	return self->inner[index] = value;													\
}																						\
void array_##name##_clear_from_size(array_##name##_t* self, size_t from, size_t size)	\
{																						\
	for(size_t i = from; i < size; i++)													\
	{																					\
		if(self->inner[i])																\
			free((void*)self->inner[i]);														\
		self->inner[i] = 0;																\
	}																					\
}																						\
void array_##name##_clear_from(array_##name##_t* self, size_t from)						\
{																						\
	array_##name##_clear_from_size(self, from, self->length - from);					\
}																						\
void array_##name##_clear(array_##name##_t* self)										\
{																						\
	array_##name##_clear_from(self, 0);													\
}																						\
void array_##name##_copy(array_##name##_t* destination, size_t destinationIndex, array_##name##_t* source, size_t sourceIndex, size_t size) \
{																										\
	if ((destination->length - destinationIndex) < size || (source->length - sourceIndex) < size)		\
		return;																							\
	memcpy(destination->inner + destinationIndex, source->inner + sourceIndex, size * sizeof(type));	\
} 

#endif
#include "shajara.h"

#include <stdio.h>
#include <stdlib.h>


Vector_t* Vector_new(size_t _capacity)
{
    Vector_t* this = malloc(sizeof(*this));
    if (!this)
    {

    }
    this->array = malloc(sizeof(*this->array) * _capacity);
    if (!this->array)
    {

    }
    return this;
}


void Vector_delete(Vector_t* _this);
void Vector_append(Vector_t* _this, void* _item);
void Vector_insert(Vector_t* _this, void* _item);
void Vector_remove(Vector_t* _this);
void* Vector_get(Vector_t* _this, size_t _index);
void Vector_set(Vector_t* _this, size_t _index, void* _item);
void Vector_peek(Vector_t* _this);
void Vector_pop(Vector_t* _this);
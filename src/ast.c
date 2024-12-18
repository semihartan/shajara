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
#include "shajara.h"

int ASTNode_compare(void* _a, void* _b)
{
	ASTNode_t* a = (ASTNode_t*)_a;
	ASTNode_t* b = (ASTNode_t*)_b;
	if (a == b)
		return 0;
	if (_tcscmp(a->name, b->name) == 0 || a->children == b->children)
		return 0;
	return -1;
}

ASTNode_t* ASTNode_new()
{
	ASTNode_t* self = (ASTNode_t*)NEWOBJP(self);
	SHJ_RETURN_E_OUTOFMEM(self, NULL);
	self->name = NULL;
	self->children = vector__alloc(ASTNode_compare);
	SHJ_RETURN_E_OUTOFMEM(self->children, NULL);
	return self;
}
ASTNode_t* ASTNode_newName(const TCHAR* _Name)
{
	ASTNode_t* self = ASTNode_new();
	SHJ_RETURN_E_OUTOFMEM(self, NULL);
	self->name = string_new(_Name);
	return self;
}

ASTNode_t* ASTNode_newString(const string_t _String)
{
	ASTNode_t* self = ASTNode_new();
	SHJ_RETURN_E_OUTOFMEM(self, NULL);
	self->name = _String;
	return self;
}

ASTNode_t* ASTNode_push(ASTNode_t* _Self, ASTNode_t* _Child)
{
	if (!_Self)
		return NULL;
	return (ASTNode_t*)vector__push_back(_Self->children, _Child);
}

ASTNode_t* ASTNode_pushName(ASTNode_t* _Self, const TCHAR* _Name)
{
	if (!_Self)
		return NULL;
	return (ASTNode_t*)vector__push_back(_Self->children, ASTNode_newName(_Name));
}

void ASTNode_print(ASTNode_t* _Self, int _Level)
{
	if (!_Self)
		return;
	for (size_t i = 0; i < _Level; i++)
	{
		_puttc(_T(' '), stdout);
		_puttc(_T(' '), stdout);
	}
	printf("+ %s\n", _Self->name);
	for (size_t i = 0; i < _Self->children->count; i++)
	{
		ASTNode_print((ASTNode_t*)vector__get(_Self->children, i), _Level+1);
	}
}

void ASTNode_delete(ASTNode_t* _Self)
{
	vector__free(_Self->children);
	free(_Self);
}
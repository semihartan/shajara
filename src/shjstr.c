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

string_t string_new(const TCHAR* _Str)
{
	size_t len = _tcslen(_Str);
	char* self = (char*)malloc((sizeof(*_Str) * (len + 1)) + sizeof(size_t));
	*(size_t*)self = len;
	self += sizeof(size_t);
	memcpy(self, _Str, sizeof(*_Str) * len);
	*((TCHAR*)self + len) = _T('\0');
	return (string_t)self;
}

string_t string_newLen(const TCHAR* _Str, size_t _Len)
{
	char* self = (char*)malloc((sizeof(*_Str) * (_Len + 1)) + sizeof(size_t));
	*(size_t*)self = _Len;
	self += sizeof(size_t);
	memcpy(self, _Str, sizeof(*_Str) * _Len);
	*((TCHAR*)self + _Len) = _T('\0');
	return (string_t)self;
}

void string_delete(const string_t _Self)
{
	free((void*)((char*)_Self - sizeof(size_t)));
}
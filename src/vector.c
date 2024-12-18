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
#include "vector.h"

DEFINE_COMPFUNC(char)
DEFINE_COMPFUNC_(unsigned_char, unsigned char)
DEFINE_COMPFUNC(short)
DEFINE_COMPFUNC_(unsigned_short, unsigned short)
DEFINE_COMPFUNC(int)
DEFINE_COMPFUNC_(unsigned_int, unsigned int)
DEFINE_COMPFUNC_(long_long, long long)
DEFINE_COMPFUNC_(unsigned_long_long, unsigned long long)

DEFINE_COMPFUNC(int8_t)
DEFINE_COMPFUNC(int16_t)
DEFINE_COMPFUNC(int32_t)
DEFINE_COMPFUNC(int64_t)
DEFINE_COMPFUNC(uint8_t)
DEFINE_COMPFUNC(uint16_t)
DEFINE_COMPFUNC(uint32_t)
DEFINE_COMPFUNC(uint64_t)

DEFINE_COMPFUNC(float)
DEFINE_COMPFUNC(double)
DEFINE_COMPFUNC_(long_double, long double)
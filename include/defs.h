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

#ifndef SHJ_DEFS_H
#define SHJ_DEFS_H  1

#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include <ctype.h>
#include <tchar.h>

#define NULLC   _T('\0')

#define NEWOBJP(p)          (malloc(sizeof(*(p))))
#define NEWOBJPN(p, n)      (malloc(sizeof(*(p)) * (n)))

#if defined(UNICODE) || defined(_UNICODE)
#   ifndef _T
#       define __T(x)       L##x    
#       define _T(x)      __T(x)
#   endif
#   define fprintf(a, b, ...)	    fwprintf(a, _T(b), __VA_ARGS__)
#   define fputs(a, b)	            fputws(_T(a), b)
#   define puts(a)	                fputws(_T(a) _T("\n"), stdout)
#   define printf(a, ...)	        wprintf(_T(a), __VA_ARGS__)
#   define printe(a, ...)	        fwprintf(stderr, _T(a) _T("\n"), __VA_ARGS__)
#else
#   ifndef _T
#       define _T(x)       x
#   endif
#   define fprintf(a, b, ...)	    fprintf(a, b, __VA_ARGS__)
#   define fputs(a, b)	            fputs(a, b) 
#   define printf(a, ...)	        printf(a, __VA_ARGS__)
#   define printe(a, ...)	        fprintf(stderr, a _T("\n"), __VA_ARGS__)
#endif

#ifdef NDEBUG
#   define SHJ_ASSERT(expr, ret, ...)    SHJ_RETURN_IF_NOT(expr, ret, __VA_ARGS__)
#else
#   define SHJ_ASSERT(expr, ret, ...)    assert(expr)
#endif

//#define SHJ_LAST_ERROR_MESSAGE              SosGetErrorMessage(GetLastError())
//#define SHJ_HRESULT_MESSAGE(hr)             SosGetErrorMessage(hr)
//#define SHJ_ERRNO_MESSAGE                   SosGetErrnoMessage(errno)
//#define SHJ_ERROR_MESSAGE_FORMAT            "An error ocurred.\nError: %s"
//#define SHJ_REPORT_LAST_ERROR()             printe(SHJ_ERROR_MESSAGE_FORMAT, SHJ_LAST_ERROR_MESSAGE)
//#define SHJ_REPORT_ERROR(hr)                printe(SHJ_ERROR_MESSAGE_FORMAT, SHJ_HRESULT_MESSAGE(hr))
//#define SHJ_REPORT_WIN32_ERROR()            printe(SHJ_ERROR_MESSAGE_FORMAT, SHJ_HRESULT_MESSAGE(SHJ_E_WIN32))
//#define SHJ_REPORT_HR_ERROR()               printe(SHJ_ERROR_MESSAGE_FORMAT, SHJ_HRESULT_MESSAGE(hr))

#define _SHJ_HANDLE_ERROR(cond, stm, ret, ...)	    \
do {                                                \
    if (!(cond)) {		                            \
        __VA_ARGS__                     	        \
        stm ret;				                    \
    }                                               \
} while (0)

#define SHJ_RETURN_IF_NOT(cond, ret, ...)           _SHJ_HANDLE_ERROR((cond), return, (ret), __VA_ARGS__)

#define SHJ_RETURN_IFN_SUCCESS(cond, ret, ...)      SHJ_RETURN_IF_NOT(ERROR_SUCCESS == (cond), (ret), __VA_ARGS__)
#define SHJ_RETURN_IF(cond, ret, ...)               SHJ_RETURN_IF_NOT(!(cond), (ret), __VA_ARGS__)
#define SHJ_RETURN_IF_NULL(cond, ret, ...)          SHJ_RETURN_IF_NOT(NULL != (cond), (ret), __VA_ARGS__)
#define SHJ_RETURN_IF_FALSE(cond, ret, ...)         SHJ_RETURN_IF_NOT((cond), (ret), __VA_ARGS__)
#define SHJ_RETURN_IF_FAILED(cond, ret, ...)        SHJ_RETURN_IF(FAILED((cond)), (ret), __VA_ARGS__)
#define SHJ_RETURN_IF_SUCCEEDED(cond, ret, ...)     SHJ_RETURN_IF(SUCCEEDED((cond)), (ret), __VA_ARGS__) 

#define SHJ_HALT_IF_NOT(cond, ...)                  _SHJ_HANDLE_ERROR((cond), exit, (EXIT_FAILURE), __VA_ARGS__)

#define SHJ_HALT_IFN_SUCCESS(cond, ...)             SHJ_HALT_IF_NOT(ERROR_SUCCESS == (cond), __VA_ARGS__)
#define SHJ_HALT_IF(cond, ...)                      SHJ_HALT_IF_NOT(!(cond), __VA_ARGS__)
#define SHJ_HALT_IF_NULL(cond, ...)                 SHJ_HALT_IF_NOT(NULL != (cond), __VA_ARGS__)
#define SHJ_HALT_IF_FALSE(cond, ...)                SHJ_HALT_IF_NOT((cond), __VA_ARGS__)
#define SHJ_HALT_IF_FAILED(cond, ...)               SHJ_HALT_IF(FAILED((cond)), __VA_ARGS__)
#define SHJ_HALT_IF_SUCCEEDED(cond, ...)            SHJ_HALT_IF(SUCCEEDED((cond)), __VA_ARGS__)

#define SHJ_GOTO_IF_NOT(cond, label, ...)           _SHJ_HANDLE_ERROR((cond), goto, label, __VA_ARGS__)

#define SHJ_GOTO_IFN_SUCCESS(cond, label, ...)      SHJ_GOTO_IF_NOT(!(cond), label, __VA_ARGS__)
#define SHJ_GOTO_IF(cond, label, ...)               SHJ_GOTO_IF_NOT(ERROR_SUCCESS == (cond), label, __VA_ARGS__)
#define SHJ_GOTO_IF_NULL(cond, label, ...)          SHJ_GOTO_IF_NOT(NULL != (cond), label, __VA_ARGS__)
#define SHJ_GOTO_IF_FALSE(cond, label, ...)         SHJ_GOTO_IF_NOT((cond), label, __VA_ARGS__)
#define SHJ_GOTO_IF_FAILED(cond, label, ...)        SHJ_GOTO_IF(FAILED((cond)), label, __VA_ARGS__)
#define SHJ_GOTO_IF_SUCCEEDED(cond, label, ...)     SHJ_GOTO_IF(SUCCEEDED((cond)), label, __VA_ARGS__)

#define SHJ_RETURN_E_OUTOFMEM(p, ret)               SHJ_RETURN_IF_NOT((p), ret, errno = ENOMEM; printe("Error: Out of memory.");)
#define SHJ_HALT_E_OUTOFMEM(p)                      SHJ_HALT_IF_NOT((p), printe("Error: Out of memory.");)
#define SHJ_GOTO_E_OUTOFMEM(p, l)                   SHJ_GOTO_IF_NOT((p), (l), printe("Error: Out of memory.");)

#endif
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

#ifndef SHJ_SHEJARA_H
#define SHJ_SHEJARA_H   1

#include "defs.h"

#include "vector.void.h"


typedef TCHAR* string_t;

string_t string_new(const TCHAR* _Str);
string_t string_newLen(const TCHAR* _Str, size_t _Len);
void string_delete(const string_t _Self);

#define STRING_LEN(str)         *(size_t*)(((char*)(str) - sizeof(size_t)))

typedef struct ASTNode_s
{
    const TCHAR* name;
    vector__t* children;
}ASTNode_t;

ASTNode_t* ASTNode_new();
ASTNode_t* ASTNode_newName(const TCHAR* _Name);
ASTNode_t* ASTNode_newString(const string_t _String);
void ASTNode_delete(ASTNode_t* _Self);
ASTNode_t* ASTNode_push(ASTNode_t* _Self, ASTNode_t* _Child);
ASTNode_t* ASTNode_pushName(ASTNode_t* _Self, const TCHAR* _Name);
void ASTNode_print(ASTNode_t* _Self, int _Level);

typedef enum
{
    SHJ_TK_INDENT,
    SHJ_TK_DEDENT,
    SHJ_TK_PLUS,
    SHJ_TK_ID,
    SHJ_TK_EOF,
}TokenKind_t;

typedef struct Token_s
{
    TokenKind_t kind;
    string_t id;
}Token_t;

void Lex_init(const TCHAR* const _Source);
Token_t Lex_NextToken();

ASTNode_t* Parser_Parse();

void Shajara_BuildDirs(ASTNode_t* _Tree, const TCHAR* _rootDir);

#endif
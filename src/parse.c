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

static ASTNode_t* _ParseDirectory();
static ASTNode_t* _ParseBlock(ASTNode_t* _parent);
static Token_t s_currentToken;
/**
    + A
    + B
    + C
    + D
        + E
        + F
 */
ASTNode_t* Parser_Parse()
{
    ASTNode_t* topLevel = ASTNode_newName(_T("."));
    s_currentToken = Lex_NextToken();
    _ParseBlock(topLevel);

    return topLevel;
}

static ASTNode_t* _ParseDirectory() 
{
    // <directory> = '+' ID
    ASTNode_t* node = NULL;
    if (s_currentToken.kind != SHJ_TK_PLUS)
        return node;
    // Skip plus token
    s_currentToken = Lex_NextToken(); 
    if (s_currentToken.kind != SHJ_TK_ID)
        return node;
    node = ASTNode_newString(s_currentToken.id);
    s_currentToken = Lex_NextToken();
    return node;
}

static ASTNode_t* _ParseBlock(ASTNode_t* _parent)
{
    ASTNode_t* child = NULL;
    while (true)
    {
        child = _ParseDirectory();
        if(child)
            vector__push_back(_parent->children, child);
        if (s_currentToken.kind == SHJ_TK_INDENT)
        {
            s_currentToken = Lex_NextToken();
            _ParseBlock(child);
        }
        else if (s_currentToken.kind == SHJ_TK_DEDENT)
        {
            s_currentToken = Lex_NextToken();
            return _parent;
        }
        else if (s_currentToken.kind == SHJ_TK_EOF)
        { 
            return _parent;
        }
    }    
    return _parent;
}
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

#define CP		s_cp
#define CC		*s_cp
#define PP		s_pp
#define PC		*s_pp
#define NC()	(*++s_cp)

#define LF			_T('\n')
#define TB			_T('\t')
#define SP			_T(' ')
#define US			_T('_')
#define DS			_T('-')
#define DT			_T('.')
#define AST			_T('*')
#define PLUS		_T('+')

#define IS_VALID_FILE_CHAR(c)	((c) == DS || (c) == US || (c) == DT || (c) == AST || _istalnum((c)))

#define NEW_TOKEN(k, i)		(Token_t){ (TokenKind_t)k, i }
 
static const TCHAR* s_source = NULL;
static const TCHAR* s_cp = NULL;
static const TCHAR* s_pp;
static struct Indentation_s { int tabs; int spaces; };

struct Indentation_s s_previousIndentation = { 0, 0 };

void Lex_init(const TCHAR* const _Source)
{
	s_source = _Source; 
	s_cp = _Source; 
	s_pp = s_cp;
}
/*
+ A
	+ B
	+ C
	+ D
		+ F
		+ G
		+ H
	+ J
	+ I
+ K
+ L
+ M
*/

Token_t Lex_NextToken()
{
	struct Indentation_s currentIndentation = { 0, 0 }; 
	for (;;)
	{ 
		// If the current char is the beginning of file or line.
		if (CP == s_source || PC == LF)
		{
			while (CC == SP || CC == TB)
			{
				currentIndentation.spaces += CC == SP;
				currentIndentation.tabs	+= CC == TB;
				NC();
			}
			if (currentIndentation.spaces + currentIndentation.tabs > s_previousIndentation.spaces + s_previousIndentation.tabs) 
			{
				s_previousIndentation = currentIndentation;
				return PP = CP, NEW_TOKEN(SHJ_TK_INDENT, NULL);
			}
			else if (currentIndentation.spaces + currentIndentation.tabs < s_previousIndentation.spaces + s_previousIndentation.tabs)
			{
				s_previousIndentation = currentIndentation;
				return PP = CP, NEW_TOKEN(SHJ_TK_DEDENT, NULL);
			}
		}
		// Skip any other whitespaces.
		while (CC == SP || CC == TB)
		{
			NC();
		}
		if (CC == PLUS)
			return PP = CP, NC(), NEW_TOKEN(SHJ_TK_PLUS, NULL);
		else if (CC == LF)
		{
			PP = CP;
			NC();
		}
		else if (IS_VALID_FILE_CHAR(CC))
		{
			const TCHAR* idStart = CP++;
			size_t len = 1;
			while (IS_VALID_FILE_CHAR(CC))
			{
				len++;
				NC();
			}
			return NEW_TOKEN(SHJ_TK_ID, string_newLen(idStart, len));
		}
		else if (CC == _T('#') || CC == _T('/'))
		{
			if (CC == _T('/'))
			{
				NC();
				if (CC == _T('/'))
					NC();
				else
					continue;
			}
			while (CC != LF) 
				NC();
		}
		else if (CC == _T('\0'))
			break;
	}
	return NEW_TOKEN(SHJ_TK_EOF, NULL);
}
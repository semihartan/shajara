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
#define HASH		_T('#')
#define FSL			_T('/')
#define BSL			_T('\\')
#define COL			_T(':')
#define EXC			_T('?')
#define PIP			_T('|')
#define LT			_T('<')
#define GT			_T('>')
#define QU			_T('\"')

#define IS_VALID_FILE_CHAR(c)	((\
(c) != SP && \
(c) != LF && \
(c) != TB && \
(c) != FSL && \
(c) != BSL && \
(c) != COL && \
(c) != EXC && \
(c) != PIP && \
(c) !=  LT && \
(c) !=  GT && \
(c) !=  QU) || \
_istalnum((c)))

#define SUMINDENT(x)				IndentationSum(x)
#define SUBINDENTS(x, y)		(struct Indentation_s){x.tabs - y.tabs, x.spaces - y.spaces}

#define NEW_TOKEN(k, i)		(Token_t){ (TokenKind_t)k, i }
 
static const TCHAR* s_source = NULL;
static const TCHAR* s_cp = NULL;
static const TCHAR* s_pp;
static struct Indentation_s { int tabs; int spaces; };


struct Indentation_s s_previousIndentation = { 0, 0 };
struct Indentation_s s_currentIndentation = { 0, 0 }; 
struct Indentation_s s_firstIndentation = { 0, 0 }; 

inline struct Indentation_s NullIndentation();
inline bool IndentationIsEmpty(struct Indentation_s);
inline int IndentationSum(struct Indentation_s indent);
inline struct Indentation_s IndentationSub(struct Indentation_s left, struct Indentation_s right);
inline bool IndentationIsEq(struct Indentation_s left, struct Indentation_s right);

void Lex_init(const TCHAR* const _Source)
{
	s_source = _Source; 
	s_cp = _Source; 
	s_pp = s_cp;
}

Token_t Lex_NextToken()
{
	for (;;)
	{
		if (CC == NULLC)
			break;
		// If the current indentation level is less than the previous, we subtract until the previous indentation
		// becomes equal to the current indentation. On each subtraction, we generate a DEDENT.
		else if (SUMINDENT(s_currentIndentation) < SUMINDENT(s_previousIndentation))
		{
			while (SUMINDENT(s_previousIndentation) > SUMINDENT(s_currentIndentation))
			{
				s_previousIndentation = IndentationSub(s_previousIndentation, s_firstIndentation);
				return PP = CP, NEW_TOKEN(SHJ_TK_DEDENT, NULL);
			}
		}
		// If the current char is the beginning of file or line.
		else if (CC == PLUS)
			return PP = CP, NC(), NEW_TOKEN(SHJ_TK_PLUS, NULL);
		else if (CC == LF)
		{
			PP = CP;
			NC();
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
			while (CC != LF && CC != NULLC)
				NC();
		}
		else if (IS_VALID_FILE_CHAR(CC) || CC == _T('"') || CC == _T('\''))
		{
			bool hasStartQuote = false;
			if (CC == _T('"') || CC == _T('\''))
			{
				hasStartQuote = true;
				NC();
			}
			const TCHAR* idStart = CP++;
			size_t len = 1;
			while (true)
			{
				if (hasStartQuote) 
				{
					if(CC == _T('"') || CC == _T('\''))
					{
						NC();
						break;
					}
					
				}
				else
				{
					if (CC == LF || CC == NULLC || CC == HASH || CC == FSL)
						break;
				}
				SHJ_HALT_IF(CC != SP && !IS_VALID_FILE_CHAR(CC),
					printe("File name must include valid file characters."););
				len++;
				NC();
			}
			string_t id = string_newLen(idStart, len); 
			for (int i = len-1; i >= 0; --i)
			{
				if (id[i] != SP)
				{
					id[i + 1] = NULLC;
					STRING_LEN(id) = i + 1;
					break;
				}
			}
			return NEW_TOKEN(SHJ_TK_ID, id);
		}
		else if (CP == s_source || CC == LF)
		{
			while (CC == LF)
		{
				NC();
			}
			s_currentIndentation = NullIndentation();
			while (CC == SP || CC == TB)
			{
				s_currentIndentation.spaces += CC == SP;
				s_currentIndentation.tabs += CC == TB;
				NC();
			} 
			if (IndentationSum(s_currentIndentation) > IndentationSum(s_previousIndentation))
			{
				if (IndentationIsEmpty(s_firstIndentation))
				{
					s_firstIndentation = s_currentIndentation;
				}
				else
					SHJ_HALT_IF(!IndentationIsEq(IndentationSub(s_currentIndentation, s_previousIndentation), s_firstIndentation),
				printe("Inconsistent tab and space uses."););
				s_previousIndentation = s_currentIndentation;
				return PP = CP, NEW_TOKEN(SHJ_TK_INDENT, NULL);
			}
		}
		else if(CC == SP || CC == TB)
		{
			// Skip any other whitespaces.
			while (CC == SP || CC == TB)
				NC(); 
		}
	}
	return NEW_TOKEN(SHJ_TK_EOF, NULL);
}

inline struct Indentation_s NullIndentation()
{
	struct Indentation_s result = { 0 };
	return result;
}
inline bool IndentationIsEmpty(struct Indentation_s indentation)
{
	return indentation.spaces == 0 && indentation.tabs == 0;
}
inline int IndentationSum(struct Indentation_s indent)
{
	return indent.spaces + indent.tabs;
}
inline struct Indentation_s IndentationSub(struct Indentation_s left, struct Indentation_s right)
{
	struct Indentation_s result = { 0 };
	result.tabs = left.tabs - right.tabs;
	result.spaces = left.spaces - right.spaces;

	return result;
};
inline bool IndentationIsEq(struct Indentation_s left, struct Indentation_s right)
{
	return left.tabs == right.tabs && left.spaces == right.spaces;
};
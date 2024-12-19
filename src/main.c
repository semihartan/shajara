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

#include <windows.h>
#include <shlwapi.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define MESSAGE_BUFFER_SZ	0xFFF

static TCHAR s_messageBuffer[MESSAGE_BUFFER_SZ + 1];
#define DEFAULT_FILE        _T("STRUCTURE")
#define NT_FILE_SEP         _T("\\")
#define UNIX_FILE_SEP       _T("/")

static const TCHAR* JointPath(int _count, ...);
static const TCHAR* ReadStructureFile(const TCHAR* _filePath);
static bool IsFileName(const TCHAR* _filePath);
static const TCHAR* GetErrnoMessage(int _Errno);
int _tmain(int argc, const TCHAR* argv[])
{
    --argc;
    ++argv;
    static TCHAR s_cwd[MAX_PATH];
    const TCHAR* structureSource = NULL;
    const TCHAR* fileName = DEFAULT_FILE;
    bool shouldFreeFileName = false;

    if (argc == 1)
    {
        fileName = argv[0];
    }

    if (IsFileName(fileName))
    {
        SHJ_HALT_IF_NULL(_tgetcwd(s_cwd, MAX_PATH),
            printe("Couldn't retrieve CWD: %s.", GetErrnoMessage(errno)););
        fileName = JointPath(2, s_cwd, fileName);
        shouldFreeFileName = true;
    }

#ifdef DEBUG
    printf("CWD: %s.\n", s_cwd);
    printf("FileName: %s.\n", fileName);
#endif // DEBUG
    


    structureSource = ReadStructureFile(fileName);

    Lex_init(structureSource);

    ASTNode_t* tree = Parser_Parse();
    ASTNode_print(tree, 0);

    Shajara_BuildDirs(tree, s_cwd);
     
    free((void*)structureSource);
    if (shouldFreeFileName)
        free((void*)fileName);
    return 0;
}

void Shajara_BuildDirs(ASTNode_t* _Tree, const TCHAR* _rootDir)
{
    static TCHAR s_filePathBuffer[MAX_PATH]; 
    const TCHAR* path = NULL;
    if (!_Tree)
        return;
    if (_tcscmp(_Tree->name, _T(".")) != 0)
    {
        _rootDir = JointPath(2, _rootDir, _Tree->name);
        if (!PathIsDirectory(_rootDir))
        {
            SHJ_HALT_IF_FAILED(_tmkdir(_rootDir),
                printe("Cound't make the directory '%s': %s.", 
                    _rootDir, GetErrnoMessage(errno)););
        }
    }
    for (size_t i = 0; i < _Tree->children->count; i++)
    {
        Shajara_BuildDirs((ASTNode_t*)vector__get(_Tree->children, i), _rootDir);
    }
    free((void*)path);
}

const TCHAR* JointPath(int _count, ...)
{
    static TCHAR s_filePathBuffer[MAX_PATH];
    va_list ap;

    if (_count <= 0)
        return NULL;
    va_start(ap, _count);
    const TCHAR* name = va_arg(ap, const TCHAR*);
    size_t pathLength = _tcslen(name);
    _tcscat_s(s_filePathBuffer, MAX_PATH, name);

    for (size_t i = 1; i < _count; i++)
    {
        name = va_arg(ap, const TCHAR*);
        _tcscat_s(s_filePathBuffer, MAX_PATH, NT_FILE_SEP);
        _tcscat_s(s_filePathBuffer, MAX_PATH, name);
        pathLength += _tcslen(name) + 1;
    }
    va_end(ap);
    const TCHAR* joinedPath = _tcsdup(s_filePathBuffer);
    SHJ_HALT_E_OUTOFMEM(joinedPath);
    s_filePathBuffer[0] = _T('\0');
    return joinedPath;
}

const TCHAR* ReadStructureFile(const TCHAR* _filePath)
{
    FILE* structureFile = _tfopen(_filePath, _T("r, ccs=UTF-8"));
    SHJ_HALT_IF_NULL(structureFile,
        printe("Couldn't open the file: %s.", _filePath););

    fseek(structureFile, 0, SEEK_END);
    size_t fileSize = (size_t)ftell(structureFile);
    fseek(structureFile, 0, SEEK_SET);

    WCHAR* fileBuffer = (WCHAR*)NEWOBJPN(fileBuffer, fileSize);
    SHJ_HALT_E_OUTOFMEM(fileBuffer);

    size_t readChars = fread_s(fileBuffer, sizeof(*fileBuffer) * fileSize, sizeof(*fileBuffer), fileSize, structureFile);
    fileBuffer[readChars] = NULLC; 
    fclose(structureFile);
    return fileBuffer;
}

static bool IsFileName(const TCHAR* _filePath)
{
    return _tcsstr(_filePath, NT_FILE_SEP) == NULL;
}

const TCHAR* GetErrnoMessage(int _Errno)
{
    return _tcserror_s(s_messageBuffer, MESSAGE_BUFFER_SZ + 1, _Errno) < 0 ? _T("_tcserror_s failed.") : s_messageBuffer;
}
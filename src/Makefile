ROOT = ..
LIBS = "kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "odbc32.lib" "shlwapi.lib" "odbccp32.lib" "powrprof.lib"

!include "$(ROOT)\target.mak"
!include "$(ROOT)\dirs.mak"

!message Output Path: $(OUT_PATH)

!if "$(SHJ_SOURCE_BROWSING)" == ""
SHJ_SOURCE_BROWSING = 0
!endif

DEBUG_CFLAGS=/JMC /D_DEBUG /DDEBUG /RTC1 /Od /MDd /ZI
DEBUG_LDLAGS=/incremental
RELEASE_CFLAGS=/DNDEBUG /GL  /Gy /O2 /Oi /MD /Zi
RELEASE_LDLAGS=/opt:ref /opt:icf /ltcg:incremental

SHARED_CFLAGS=/Fd:$(OBJDIR)\vc.pdb /DSHJ_SHARED_LIB /D_WINDOWS /D_USRDLL
SHARED_LDFLAGS=/subsystem:windows /DLL /IMPLIB:$(OUT_DIR)\$(OUT_NAME).lib $(LIBS)

STATIC_CFLAGS=/Fd:$(OUT_DIR)\$(OUT_NAME).pdb /DSHJ_STATIC_LIB /D_LIB
STATIC_LDFLAGS=/machine:$(SHJ_TARGET_ARCH) /nologo

EXECUTABLE_CFLAGS=/Fd:$(OBJDIR)\vc.pdb /DSHJ_EXECUTABLE /D_CONSOLE
EXECUTABLE_LDFLAGS=/subsystem:console $(LIBS)

CFLAGS=/W4 /I$(INCDIR)\ /D_UNICODE /DUNICODE /DWIN32_LEAN_AND_MEAN /D_WIN32_WINNT=0x0A00 \
/permissive- /ifcOutput $(OBJDIR)\ /GS /Zc:wchar_t /Gm- \
/sdl /Zc:inline /fp:precise /errorReport:prompt /WX- /Zc:forScope \
/Gd /FC /EHsc /nologo /diagnostics:column /TC

LDFLAGS=/manifest:embed /nxcompat /pdb:$(OUT_DIR)\$(OUT_NAME).pdb /dynamicbase \
/debug /machine:$(SHJ_TARGET_ARCH) \
/manifestuac:"level='asInvoker' uiAccess='false'" \
/ltcgout:"$(OBJDIR)\$(OUT_NAME).iobj" \
/errorreport:prompt /ilk:"$(OBJDIR)\$(OUT_NAME).ilk" /nologo /tlbid:1 

##############################################################################
#	Object File Definitions
##############################################################################

OBJS_STATIC_LIB =
OBJS_SHARED_LIB =

OBJS_EXE = \
$(OBJDIR)\array.void.obj \
$(OBJDIR)\vector.obj \
$(OBJDIR)\vector.int.obj \
$(OBJDIR)\vector.void.obj \
$(OBJDIR)\ast.obj \
$(OBJDIR)\lex.obj \
$(OBJDIR)\parse.obj \
$(OBJDIR)\shjstr.obj \
$(OBJDIR)\main.obj 

##############################################################################


##############################################################################
#    Conditionals
##############################################################################

!if $(SHJ_DEBUG)==1
CFLAGS=$(CFLAGS) $(DEBUG_CFLAGS)
LDFLAGS=$(LDFLAGS) $(DEBUG_LDLAGS)
!else
CFLAGS=$(CFLAGS) $(RELEASE_CFLAGS) 
LDFLAGS=$(LDFLAGS) $(RELEASE_LDLAGS)
!endif

!if defined(SHJ_SHARED_LIB)
CFLAGS=$(CFLAGS) $(SHARED_CFLAGS)
LD=link
LDFLAGS=$(LDFLAGS) $(SHARED_LDFLAGS)
OBJS=$(OBJS_SHARED_LIB)
!elseif defined(SHJ_STATIC_LIB)
CFLAGS=$(CFLAGS) $(STATIC_CFLAGS)
LD=lib
LDFLAGS=$(STATIC_LDFLAGS)
OBJS=$(OBJS_STATIC_LIB)
!else
CFLAGS=$(CFLAGS) $(EXECUTABLE_CFLAGS)
LD=link
LDFLAGS=$(LDFLAGS) $(EXECUTABLE_LDFLAGS)
OBJS=$(OBJS_EXE)
!endif

PCH_FLAGS=

!if "$(USE_PCH)"=="1"
OBJS=$(OBJDIR)\pch.obj $(OBJS)
CFLAGS=$(CFLAGS) /Fp$(OBJDIR)\$(OUT_NAME).pch
PCH_FLAGS= /Yupch.h
!endif

##############################################################################

.SUFFIXES: .c .h .obj

.c{$(OBJDIR)}.obj:
	@echo  Compiling $<...
    $(CC) $(CFLAGS) /Fo$(OBJDIR)\ $(PCH_FLAGS) /c $<

##############################################################################

all: dirs \
    $(OUT_PATH) \
!IF $(SHJ_SOURCE_BROWSING)==1
    $(OBJDIR)\$(OUT_NAME).bsc \
!endif

##############################################################################

clean:
    -del *~ 2>nul
    -del $(OUT_PATH) 2>nul
    -del $(OUT_DIR)\$(OUT_NAME).pdb 2>nul
    -rmdir /q /s ..\obj 2>nul
    
shadowclean:
    -del *~ 2>nul 
    -del $(OBJS_SHARED_LIB) 2>nul
    -rmdir /q /s obj 2>nul

##############################################################################

dirs:
    @if not exist "$(BINDIR)" mkdir "$(BINDIR)" && echo.   Created $(BINDIR)
    @if not exist "$(LIBDIR)" mkdir "$(LIBDIR)" && echo.   Created $(LIBDIR)
    @if not exist "$(OBJDIR)" mkdir "$(OBJDIR)" && echo.   Created $(OBJDIR)


$(OBJDIR)\$(OUT_NAME).bsc : $(OBJS)
    bscmake /v /n /o $@ $(OBJS:.obj=.sbr)

$(OUT_PATH): $(OBJS)
    $(LD) /out:"$@" $(LDFLAGS) $**  

!if  defined(USE_PCH)
$(OBJDIR)\pch.obj: $(INCDIR)\framework.h $(INCDIR)\defs.h $(INCDIR)\pch.h
    @echo.  Creating precompiled header file...
    $(CC) $(CFLAGS) /Fo$(OBJDIR)\ /Ycpch.h /c pch.c 
!endif

$(INCDIR)\shajara.h: $(INCDIR)\defs.h
$(INCDIR)\array.void.h: $(INCDIR)\array.h
$(INCDIR)\vector.h: $(INCDIR)\array.h
$(INCDIR)\vector.int.h: $(INCDIR)\vector.h
$(INCDIR)\vector.void.h: $(INCDIR)\vector.h $(INCDIR)\array.void.h

$(OBJDIR)\array.void.obj: \
array.void.c \
$(INCDIR)\array.void.h

$(OBJDIR)\vector.void.obj: \
vector.void.c \
$(INCDIR)\vector.void.h

$(OBJDIR)\vector.int.obj: \
vector.int.c \
$(INCDIR)\vector.int.h

$(OBJDIR)\vector.obj: \
vector.c \
$(INCDIR)\vector.h

$(OBJDIR)\shjstr.obj: \
shjstr.c \
$(INCDIR)\shajara.h

$(OBJDIR)\lex.obj: \
lex.c \
$(INCDIR)\shajara.h

$(OBJDIR)\ast.obj: \
ast.c \
$(INCDIR)\shajara.h

$(OBJDIR)\parse.obj: \
parse.c \
$(INCDIR)\shajara.h

$(OBJDIR)\main.obj: \
main.c \
$(INCDIR)\shajara.h

##############################################################################
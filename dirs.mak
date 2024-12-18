INCDIR = $(ROOT)\include
LIBDIR = $(ROOT)\lib\$(SHJ_CONFIG)\$(SHJ_TARGET_ARCH)
BINDIR = $(ROOT)\bin\$(SHJ_CONFIG)\$(SHJ_TARGET_ARCH)
RESDIR = $(ROOT)\res

INCDIRS = $(ROOT)\include

LIBDIRS = \
$(ROOT)\lib\$(SHJ_CONFIG)\x86 \
$(ROOT)\lib\$(SHJ_CONFIG)\x64 \
$(ROOT)\lib\$(SHJ_CONFIG)\arm \
$(ROOT)\lib\$(SHJ_CONFIG)\arm64

BINDIRS = \
$(ROOT)\bin\$(SHJ_CONFIG)\x86 \
$(ROOT)\bin\$(SHJ_CONFIG)\x64 \
$(ROOT)\bin\$(SHJ_CONFIG)\arm \
$(ROOT)\bin\$(SHJ_CONFIG)\arm64

OBJDIRS = \
obj\$(SHJ_CONFIG)\x86 \
obj\$(SHJ_CONFIG)\x64 \
obj\$(SHJ_CONFIG)\arm \
obj\$(SHJ_CONFIG)\arm64 \

!if "$(SHJ_SHARED_LIB)"=="1"
OUT_EXT=dll
OUT_DIR=$(BINDIR)
OBJDIR = $(ROOT)\obj\lib\shared\$(SHJ_CONFIG)\$(SHJ_TARGET_ARCH)
!elseif "$(SHJ_STATIC_LIB)"=="1"
OUT_EXT=lib
OUT_DIR=$(LIBDIR)
OBJDIR = $(ROOT)\obj\lib\static\$(SHJ_CONFIG)\$(SHJ_TARGET_ARCH)
!else
OUT_EXT=exe
OUT_DIR=$(BINDIR)
OBJDIR = $(ROOT)\obj\bin\$(SHJ_CONFIG)\$(SHJ_TARGET_ARCH)
!endif
 
OUT_PATH=$(OUT_DIR)\$(OUT_NAME).$(OUT_EXT)
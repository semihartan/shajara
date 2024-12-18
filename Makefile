ROOT = .

PROJECT_NAME=shajara
OUT_NAME=$(PROJECT_NAME)

!message Project Name: $(PROJECT_NAME)

USE_PCH=0

!if "$(USE_PCH)"==""
USE_PCH=1
!message Using Precompiled Header...
!endif

SHJ_SHARED_LIB=$(SHJ_SHARED_LIB)
SHJ_STATIC_LIB=$(SHJ_STATIC_LIB)
SHJ_TARGET_ARCH=$(SHJ_TARGET_ARCH)
SHJ_CONFIG=$(SHJ_CONFIG)

!include "$(ROOT)\target.mak"
!include "$(ROOT)\dirs.mak"

!message    SHJ_SHARED_LIB=$(SHJ_SHARED_LIB)
!message    SHJ_STATIC_LIB=$(SHJ_STATIC_LIB)
!message    SHJ_TARGET_ARCH=$(SHJ_TARGET_ARCH)
!message    SHJ_CONFIG=$(SHJ_CONFIG)

all:
    cd "$(MAKEDIR)"
	set PROJECT_NAME=$(PROJECT_NAME)
	set OUT_NAME=$(OUT_NAME)
	set USE_PCH=$(USE_PCH)
	@echo "    $(MAKE)"
	@echo "    Entering src.."
	cd "$(MAKEDIR)\src"
!if "$(SHJ_SHARED_LIB)"=="1"
	@echo "    Building shared lib.."
	@echo "    Calling src\makefile.."
!elseif "$(SHJ_STATIC_LIB)"=="1"
	@echo "    Building static lib.."
	@echo "    Calling src\makefile.."
!else
	@echo "    Building executable.."
	@echo "    Calling src\makefile.."
!endif
    @$(MAKE) /NOLOGO $(MAKEFLAGS)
	@echo "    Exiting src.."
	cd "$(MAKEDIR)"
	
clean:
	cd "$(MAKEDIR)"
	cd "$(MAKEDIR)\src"
	set PROJECT_NAME=$(PROJECT_NAME)
	set OUT_NAME=$(OUT_NAME)
	set USE_PCH=$(USE_PCH)
    @$(MAKE) /NOLOGO $(MAKEFLAGS) clean
	cd "$(MAKEDIR)"
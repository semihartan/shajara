!if "$(SHJ_TARGET_ARCH)" == "" && "$(VSCMD_ARG_TGT_ARCH)" != ""
SHJ_TARGET_ARCH=$(VSCMD_ARG_TGT_ARCH)
!endif

!if "$(SHJ_TARGET_ARCH)"==""
SHJ_TARGET_ARCH=x64
!else
# Lowercase SHJ_TARGET_ARCH
SHJ_TARGET_ARCH=$(SHJ_TARGET_ARCH:A=a)
SHJ_TARGET_ARCH=$(SHJ_TARGET_ARCH:B=b)
SHJ_TARGET_ARCH=$(SHJ_TARGET_ARCH:C=c)
SHJ_TARGET_ARCH=$(SHJ_TARGET_ARCH:D=d)
SHJ_TARGET_ARCH=$(SHJ_TARGET_ARCH:E=e)
SHJ_TARGET_ARCH=$(SHJ_TARGET_ARCH:F=f)
SHJ_TARGET_ARCH=$(SHJ_TARGET_ARCH:G=g)
SHJ_TARGET_ARCH=$(SHJ_TARGET_ARCH:H=h)
SHJ_TARGET_ARCH=$(SHJ_TARGET_ARCH:I=i)
SHJ_TARGET_ARCH=$(SHJ_TARGET_ARCH:J=j)
SHJ_TARGET_ARCH=$(SHJ_TARGET_ARCH:K=k)
SHJ_TARGET_ARCH=$(SHJ_TARGET_ARCH:L=l)
SHJ_TARGET_ARCH=$(SHJ_TARGET_ARCH:M=m)
SHJ_TARGET_ARCH=$(SHJ_TARGET_ARCH:N=n)
SHJ_TARGET_ARCH=$(SHJ_TARGET_ARCH:O=o)
SHJ_TARGET_ARCH=$(SHJ_TARGET_ARCH:P=p)
SHJ_TARGET_ARCH=$(SHJ_TARGET_ARCH:Q=q)
SHJ_TARGET_ARCH=$(SHJ_TARGET_ARCH:R=r)
SHJ_TARGET_ARCH=$(SHJ_TARGET_ARCH:S=s)
SHJ_TARGET_ARCH=$(SHJ_TARGET_ARCH:T=t)
SHJ_TARGET_ARCH=$(SHJ_TARGET_ARCH:U=u)
SHJ_TARGET_ARCH=$(SHJ_TARGET_ARCH:V=v)
SHJ_TARGET_ARCH=$(SHJ_TARGET_ARCH:W=w)
SHJ_TARGET_ARCH=$(SHJ_TARGET_ARCH:X=x)
SHJ_TARGET_ARCH=$(SHJ_TARGET_ARCH:Y=y)
SHJ_TARGET_ARCH=$(SHJ_TARGET_ARCH:Z=z)

!if "$(SHJ_TARGET_ARCH)"=="amd64"
SHJ_TARGET_ARCH=x64
!endif

!if "$(SHJ_TARGET_ARCH)"=="win32"
SHJ_TARGET_ARCH=x86
!endif

!endif

!if "$(SHJ_TARGET_ARCH:64=)" == "$(SHJ_TARGET_ARCH)"
SHJ_32BIT=1
SHJ_BITS=32
!else
SHJ_64BIT=1
SHJ_BITS=64
!endif

!IF "$(SHJ_CONFIG)" == ""
SHJ_CONFIG=Release
!ENDIF

!IF "$(SHJ_CONFIG)" == "Debug"
SHJ_DEBUG=1
!ELSE
SHJ_DEBUG=0
!ENDIF

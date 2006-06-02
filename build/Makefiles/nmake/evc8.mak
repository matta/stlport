# build/Makefiles/nmake/evc8.mak

# Note: _WIN32_WCE is defined as 420 for CE 4.2 but as 0x500 for CE 5.0!
DEFS_COMMON = $(DEFS_COMMON) /D _WIN32_WCE=0x$(CEVERSION) /D UNDER_CE=1 /D "UNICODE"
LDFLAGS_COMMON = $(LDFLAGS_COMMON) commctrl.lib coredll.lib corelibc.lib /nodefaultlib:LIBC.lib /nodefaultlib:OLDNAMES.lib 
# TODO: the subsystem settings will have to be adjusted for CE5.01...
LDFLAGS_COMMON = $(LDFLAGS_COMMON) /subsystem:windowsce,5.00

!if "$(TARGET_PROC)" == ""
!error No target processor configured! Please rerun configure.bat!
!endif

!if "$(CC)" == ""
CC=cl.exe
!endif

CXX = $(CC)

# ARM specific settings
!if "$(TARGET_PROC)" == "arm"
DEFS_COMMON = $(DEFS_COMMON) /D "ARM" /D "_ARM_" /D "$(TARGET_PROC_SUBTYPE)"
OPT_COMMON = $(OPT_COMMON)
!endif

# Note: /GX for MSC<14 has been replaced with /EHsc
CFLAGS_COMMON = /nologo /TC /W4 /GF /GR /EHsc
CFLAGS_REL = $(CFLAGS_COMMON) $(OPT_REL)
CFLAGS_STATIC_REL = $(CFLAGS_COMMON) $(OPT_STATIC_REL)
CFLAGS_DBG = $(CFLAGS_COMMON) $(OPT_DBG)
CFLAGS_STATIC_DBG = $(CFLAGS_COMMON) $(OPT_STATIC_DBG)
CFLAGS_STLDBG = $(CFLAGS_COMMON) $(OPT_STLDBG)
CFLAGS_STATIC_STLDBG = $(CFLAGS_COMMON) $(OPT_STATIC_STLDBG)
CXXFLAGS_COMMON = /nologo /TP /W4 /GF /GR /EHsc
CXXFLAGS_REL = $(CXXFLAGS_COMMON) $(OPT_REL)
CXXFLAGS_STATIC_REL = $(CXXFLAGS_COMMON) $(OPT_STATIC_REL)
CXXFLAGS_DBG = $(CXXFLAGS_COMMON) $(OPT_DBG)
CXXFLAGS_STATIC_DBG = $(CXXFLAGS_COMMON) $(OPT_STATIC_DBG)
CXXFLAGS_STLDBG = $(CXXFLAGS_COMMON) $(OPT_STLDBG)
CXXFLAGS_STATIC_STLDBG = $(CXXFLAGS_COMMON) $(OPT_STATIC_STLDBG)

# setup proper runtime (static/dynamic, debug/release)
!ifdef STLP_BUILD_FORCE_STATIC_RUNTIME
OPT_DBG = $(OPT_DBG) /MTd
OPT_STLDBG = $(OPT_STLDBG) /MTd
OPT_REL = $(OPT_REL) /MT
DEFS_REL = $(DEFS_REL) /D_STLP_USE_DYNAMIC_LIB
DEFS_DBG = $(DEFS_DBG) /D_STLP_USE_DYNAMIC_LIB
DEFS_STLDBG = $(DEFS_STLDBG) /D_STLP_USE_DYNAMIC_LIB
!else
OPT_DBG = $(OPT_DBG) /MDd
OPT_STLDBG = $(OPT_STLDBG) /MDd
OPT_REL = $(OPT_REL) /MD
!endif


!include evc-common.mak

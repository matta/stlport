# -*- makefile -*- Time-stamp: <04/03/31 08:10:19 ptr>
# $Id$

LDFLAGS_COMMON = commctrl.lib coredll.lib corelibc.lib ccrtrtti.lib /nologo /base:"0x00100000" /stack:0x10000,0x1000 /incremental:no /nodefaultlib:"LIBC.lib" /subsystem:WINDOWSCE /align:"4096"

!if "$(TARGET_PROC)" == "arm"
LDFLAGS_COMMON = $(LDFLAGS_COMMON) /MACHINE:ARM
!endif

!if "$(TARGET_PROC)" == "x86"
LDFLAGS_COMMON = $(LDFLAGS_COMMON) $(CEx86Corelibc) /nodefaultlib:"OLDNAMES.lib" /MACHINE:IX86
!endif

!include evc-common.mak

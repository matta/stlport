# -*- Makefile -*- Time-stamp: <04/03/31 00:35:47 ptr>
# $Id$

SRCROOT=.

STLPORT_INCLUDE_DIR = ../../stlport
!include Makefile.inc

CROSS_COMPILING=1

DEFS_REL = /D_STLP_USE_DYNAMIC_LIB
DEFS_DBG = /D_STLP_USE_DYNAMIC_LIB
DEFS_STLDBG = /D_STLP_USE_DYNAMIC_LIB
DEFS_STATIC_REL = /D_STLP_USE_STATIC_LIB
DEFS_STATIC_DBG = /D_STLP_USE_STATIC_LIB
DEFS_STATIC_STLDBG = /D_STLP_USE_STATIC_LIB

INCLUDES=$(INCLUDES) /I "$(STLPORT_INCLUDE_DIR)" /FI "vc_warning_disable.h"

LDSEARCH=$(LDSEARCH) /LIBPATH:$(STLPORT_LIB_DIR)
RC_FLAGS_REL = /d COMP=$(COMPILER_NAME)
RC_FLAGS_DBG = /d COMP=$(COMPILER_NAME) /d BUILD=_DEBUG
RC_FLAGS_STLDBG = /d COMP=$(COMPILER_NAME) /d BUILD=_STLDEBUG
OPT_STLDBG = /Zm800
OPT_STLDBG_STATIC = /Zm800

!include $(SRCROOT)/Makefiles/nmake/top.mak

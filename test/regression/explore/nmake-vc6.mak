# -*- Makefile -*- Time-stamp: <03/10/26 23:31:13 ptr>
# $Id$

SRCROOT=../../../src/explore
COMPILER_NAME=vc6
STLPORT_DIR=../../..

!include Makefile.inc

INCLUDES=$(INCLUDES) /I "$(STLPORT_INCLUDE_DIR)"
DEFS = /D_STLP_USE_STATIC_LIB

LDSEARCH=$(LDSEARCH) /LIBPATH:$(STLPORT_LIB_DIR)

LDLIBS_REL=stlport_$(COMPILER_NAME)_static.lib
LDLIBS_DBG=stlport_$(COMPILER_NAME)debug_static.lib
LDLIBS_STLDBG=stlport_$(COMPILER_NAME)_stldebug_static.lib

!include $(SRCROOT)/Makefiles/nmake/top.mak

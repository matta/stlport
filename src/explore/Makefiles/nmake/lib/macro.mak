# -*- makefile -*- Time-stamp: <04/02/06 01:40:00 ptr>
# $Id$

DBG_SUFFIX = debug

# Shared libraries:

SO_NAME        = $(LIBNAME)_$(COMPILER_NAME).$(SO)
LIB_NAME        = $(LIBNAME)_$(COMPILER_NAME).$(LIBEXT)
EXP_NAME        = $(LIBNAME)_$(COMPILER_NAME).$(EXP)

SO_NAME_OUT    = $(OUTPUT_DIR)\$(SO_NAME)
LIB_NAME_OUT    = $(OUTPUT_DIR)\$(LIB_NAME)
EXP_NAME_OUT    = $(OUTPUT_DIR)\$(EXP_NAME)

SO_NAME_DBG    = $(LIBNAME)_$(COMPILER_NAME)$(DBG_SUFFIX).$(SO)
LIB_NAME_DBG    = $(LIBNAME)_$(COMPILER_NAME)$(DBG_SUFFIX).$(LIBEXT)
EXP_NAME_DBG    = $(LIBNAME)_$(COMPILER_NAME)$(DBG_SUFFIX).$(EXP)

SO_NAME_OUT_DBG    = $(OUTPUT_DIR_DBG)\$(SO_NAME_DBG)
LIB_NAME_OUT_DBG    = $(OUTPUT_DIR_DBG)\$(LIB_NAME_DBG)
EXP_NAME_OUT_DBG    = $(OUTPUT_DIR_DBG)\$(EXP_NAME_DBG)

SO_NAME_STLDBG    = $(LIBNAME)_$(COMPILER_NAME)_stl$(DBG_SUFFIX).$(SO)
LIB_NAME_STLDBG    = $(LIBNAME)_$(COMPILER_NAME)_stl$(DBG_SUFFIX).$(LIBEXT)
EXP_NAME_STLDBG    = $(LIBNAME)_$(COMPILER_NAME)_stl$(DBG_SUFFIX).$(EXP)

SO_NAME_OUT_STLDBG   = $(OUTPUT_DIR_STLDBG)\$(SO_NAME_STLDBG)
LIB_NAME_OUT_STLDBG    = $(OUTPUT_DIR_STLDBG)\$(LIB_NAME_STLDBG)
EXP_NAME_OUT_STLDBG    = $(OUTPUT_DIR_STLDBG)\$(EXP_NAME_STLDBG)

# Static libraries:
A_NAME = $(LIBNAME)_$(COMPILER_NAME)static.$(ARCH)
A_NAME_OUT = $(OUTPUT_DIR_A)\$(A_NAME)

A_NAME_DBG = $(LIBNAME)_$(COMPILER_NAME)$(DBG_SUFFIX)static.$(ARCH)
A_NAME_OUT_DBG = $(OUTPUT_DIR_A_DBG)\$(A_NAME_DBG)

A_NAME_STLDBG = $(LIBNAME)_$(COMPILER_NAME)_stl$(DBG_SUFFIX)static.$(ARCH)
A_NAME_OUT_STLDBG = $(OUTPUT_DIR_A_STLDBG)\$(A_NAME_STLDBG)

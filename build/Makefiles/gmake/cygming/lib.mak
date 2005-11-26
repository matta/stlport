# -*- makefile -*- Time-stamp: <03/07/15 18:23:04 ptr>

ifneq (dmc, $(COMPILER_NAME))
ifneq (bcc, $(COMPILER_NAME))
LIB_PREFIX := lib
endif
endif

LIB_PREFIX ?=

ifneq (bcc, $(COMPILER_NAME))
DBG_SUFFIX := g
else
DBG_SUFFIX := d
endif

ifneq (dmc, $(COMPILER_NAME))
VERSION_EXT := .${MAJOR}.${MINOR}
else
#VERSION_EXT := ${MAJOR}${MINOR}
endif

STLDBG_SUFFIX := stl${DBG_SUFFIX}

# Shared libraries:
SO_NAME_BASE := $(LIB_PREFIX)${LIBNAME}${VERSION_EXT}
SO_NAME        := ${SO_NAME_BASE}.$(SO)
LIB_NAME        := ${SO_NAME_BASE}.$(LIB)
#EXP_NAME        := ${SO_NAME_BASE}.$(EXP)

SO_NAME_OUT    := $(OUTPUT_DIR)/${SO_NAME}
LIB_NAME_OUT    := $(OUTPUT_DIR)/${LIB_NAME}
#EXP_NAME_OUT    := $(OUTPUT_DIR)/${EXP_NAME}

SO_NAME_DBG_BASE := $(LIB_PREFIX)${LIBNAME}${DBG_SUFFIX}${VERSION_EXT}
SO_NAME_DBG    := ${SO_NAME_DBG_BASE}.$(SO)
LIB_NAME_DBG    := ${SO_NAME_DBG_BASE}.$(LIB)
#EXP_NAME_DBG    := ${SO_NAME_DBG_BASE}.$(EXP)

SO_NAME_OUT_DBG    := $(OUTPUT_DIR_DBG)/${SO_NAME_DBG}
LIB_NAME_OUT_DBG    := $(OUTPUT_DIR_DBG)/${LIB_NAME_DBG}
#EXP_NAME_OUT_DBG    := $(OUTPUT_DIR_DBG)/${EXP_NAME_DBG}

SO_NAME_STLDBG_BASE := $(LIB_PREFIX)${LIBNAME}${STLDBG_SUFFIX}${VERSION_EXT}
SO_NAME_STLDBG    := ${SO_NAME_STLDBG_BASE}.$(SO)
LIB_NAME_STLDBG    := ${SO_NAME_STLDBG_BASE}.$(LIB)
#EXP_NAME_STLDBG    := ${SO_NAME_STLDBG_BASE}.$(EXP)

SO_NAME_OUT_STLDBG    := $(OUTPUT_DIR_STLDBG)/${SO_NAME_STLDBG}
LIB_NAME_OUT_STLDBG    := $(OUTPUT_DIR_STLDBG)/${LIB_NAME_STLDBG}
#EXP_NAME_OUT_STLDBG    := $(OUTPUT_DIR_STLDBG)/${EXP_NAME_STLDBG}

# Static libraries:
A_NAME := ${SO_NAME_BASE}.$(ARCH)
A_NAME_OUT := $(OUTPUT_DIR_A)/$(A_NAME)

A_NAME_DBG := ${SO_NAME_DBG_BASE}.$(ARCH)
A_NAME_OUT_DBG := $(OUTPUT_DIR_A_DBG)/$(A_NAME_DBG)

A_NAME_STLDBG := ${SO_NAME_STLDBG_BASE}.$(ARCH)
A_NAME_OUT_STLDBG := $(OUTPUT_DIR_A_STLDBG)/$(A_NAME_STLDBG)

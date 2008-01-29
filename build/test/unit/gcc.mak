# -*- Makefile -*- Time-stamp: <08/01/29 09:48:15 ptr>

SRCROOT := ../..
COMPILER_NAME := gcc
#NOT_USE_NOSTDLIB := 1
#WITHOUT_STLPORT := 1
-include ${SRCROOT}/Makefiles/gmake/config.mak
ALL_TAGS = release-shared
ifndef WITHOUT_STLPORT
ALL_TAGS += stldbg-shared
endif
STLPORT_DIR := ../../..

include Makefile.inc
include ${SRCROOT}/Makefiles/gmake/top.mak

release-shared: STLPORT_LIB_DIR = ${PWD}/../../lib/${OUTPUT_DIR}
dbg-shared:     STLPORT_LIB_DIR = ${PWD}/../../lib/${OUTPUT_DIR_DBG}
ifndef WITHOUT_STLPORT
stldbg-shared:  STLPORT_LIB_DIR = ${PWD}/../../lib/${OUTPUT_DIR_STLDBG}
endif

ifdef WITHOUT_STLPORT
DEFS += -DWITHOUT_STLPORT
endif

dbg-shared:	DEFS += -D_STLP_DEBUG_UNINITIALIZED
ifndef WITHOUT_STLPORT
stldbg-shared:	DEFS += -D_STLP_DEBUG_UNINITIALIZED
endif

ifdef STLP_BUILD_BOOST_PATH
INCLUDES += -I${STLP_BUILD_BOOST_PATH}
endif

ifndef TARGET_OS
ifndef WITHOUT_STLPORT
ifeq ($(OSNAME), sunos)
LDFLAGS += -Wl,-R${STLPORT_LIB_DIR}
endif
ifeq ($(OSNAME), freebsd)
LDFLAGS += -Wl,-R${STLPORT_LIB_DIR}
endif
ifeq ($(OSNAME), openbsd)
LDFLAGS += -Wl,-R${STLPORT_LIB_DIR}
endif
ifeq ($(OSNAME), linux)
LDFLAGS += -Wl,-rpath=${STLPORT_LIB_DIR}
endif
ifeq ($(OSNAME), hp-ux)
LDFLAGS += -Wl,+b${STLPORT_LIB_DIR}
endif
endif
endif

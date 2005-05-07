# Time-stamp: <05/05/06 17:25:20 ptr>
# $Id$


#INCLUDES = -I$(SRCROOT)/include
INCLUDES :=

CXX := mwccnlm
CC := mwccnlm
LINK.cc = mwldnlm $(LDFLAGS) $(TARGET_ARCH)

ifeq ($(OSNAME), cygming)
RC := windres
endif

CXX_VERSION := $(shell ${CXX} --version | grep ${CXX} | awk '{ print $$3; }')
ifeq ($(CXX_VERSION),)
CXX_VERSION := $(shell ${CXX} --version)
endif
CXX_VERSION_MAJOR := $(shell echo ${CXX_VERSION} | awk 'BEGIN { FS = "."; } { print $$1; }')
CXX_VERSION_MINOR := $(shell echo ${CXX_VERSION} | awk 'BEGIN { FS = "."; } { print $$2; }')
CXX_VERSION_PATCH := $(shell echo ${CXX_VERSION} | awk 'BEGIN { FS = "."; } { print $$3; }')

DEFS ?=
OPT ?=

OUTPUT_OPTION = -o $@
LINK_OUTPUT_OPTION = ${OUTPUT_OPTION}
CPPFLAGS = $(DEFS) -gccinc -I. $(INCLUDES) -ir "$(MWCW_NOVELL_SDK)" -ir "$(MWCW_NOVELL_R)" -prefix Headers/nlm_prefix.h

ifeq ($(OSNAME), cygming)
release-shared : RCFLAGS = --include-dir=${STLPORT_INCLUDE_DIR} -DCOMP=gcc -DBUILD=r -DBUILD_INFOS="-O2" --output-format coff
dbg-shared : RCFLAGS = --include-dir=${STLPORT_INCLUDE_DIR} -DCOMP=gcc -DBUILD=d -DBUILD_INFOS="-g" --output-format coff
stldbg-shared : RCFLAGS = --include-dir=${STLPORT_INCLUDE_DIR} -DCOMP=gcc -DBUILD=stld -DBUILD_INFOS="-g -D_STLP_DEBUG" --output-format coff
RC_OUTPUT_OPTION = -o $@
COMPILE.rc = $(RC) $(RCFLAGS)
endif

CFLAGS = -lang c -msgstyle gcc -ext o -nostdinc -flag longlong_prepeval $(OPT)
CXXFLAGS = -lang c++ -msgstyle gcc -ext o -iso_templates on -bool on -Cpp_exceptions on -wchar_t on -nostdinc -flag longlong_prepeval $(OPT)

ifdef EXTRA_CXXFLAGS
CXXFLAGS += ${EXTRA_CXXFLAGS}
endif

CDEPFLAGS = -M
CCDEPFLAGS = -M

# STLport DEBUG mode specific defines
stldbg-static :	    DEFS += -D_STLP_DEBUG
stldbg-shared :     DEFS += -D_STLP_DEBUG
stldbg-static-dep : DEFS += -D_STLP_DEBUG
stldbg-shared-dep : DEFS += -D_STLP_DEBUG

# optimization and debug compiler flags
release-static : OPT += -O4
release-shared : OPT += -O4

dbg-static : OPT += -g
dbg-shared : OPT += -g
#dbg-static-dep : OPT += -g
#dbg-shared-dep : OPT += -g

stldbg-static : OPT += -g
stldbg-shared : OPT += -g
#stldbg-static-dep : OPT += -g
#stldbg-shared-dep : OPT += -g

# dependency output parser (dependencies collector)

DP_OUTPUT_DIR = | sed 's|\($*\)\.o[ :]*|$(OUTPUT_DIR)/\1.o $@ : |g' > $@; \
                           [ -s $@ ] || rm -f $@

DP_OUTPUT_DIR_DBG = | sed 's|\($*\)\.o[ :]*|$(OUTPUT_DIR_DBG)/\1.o $@ : |g' > $@; \
                           [ -s $@ ] || rm -f $@

DP_OUTPUT_DIR_STLDBG = | sed 's|\($*\)\.o[ :]*|$(OUTPUT_DIR_STLDBG)/\1.o $@ : |g' > $@; \
                           [ -s $@ ] || rm -f $@


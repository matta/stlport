# -*- Makefile -*- Time-stamp: <03/10/12 20:35:49 ptr>
# $Id$

SRCROOT := .
COMPILER_NAME := vc6

STLPORT_INCLUDE_DIR = ../../stlport
include Makefile.inc
include ${SRCROOT}/Makefiles/top.mak


INCLUDES += /I $(STLPORT_INCLUDE_DIR) /FI vc_warning_disable.h

# options for build with boost support
ifdef STLP_BUILD_BOOST_PATH
INCLUDES += /I$(STLP_BUILD_BOOST_PATH)
endif


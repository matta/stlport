# Time-stamp: <04/03/31 08:06:57 ptr>
# $Id$

# dependency output parser
#!include ${RULESBASE}/dparser-$(COMPILER_NAME).mak

# if sources disposed in several dirs, calculate
# appropriate rules; here is recursive call!

#DIRS_UNIQUE_SRC := $(dir $(SRC_CPP) $(SRC_CC) $(SRC_C) )
#DIRS_UNIQUE_SRC := $(sort $(DIRS_UNIQUE_SRC) )
#include ${RULESBASE}/dirsrc.mak
!include $(RULESBASE)/$(USE_MAKE)/rules-o.mak

#ALLBASE    := $(basename $(notdir $(SRC_CC) $(SRC_CPP) $(SRC_C)))
ALLBASE    = $(SRC_CC) $(SRC_CPP) $(SRC_C)
#ALLOBJS    := $(addsuffix .o,$(ALLBASE))

# follow tricks to avoid leading space if one of the macro undefined:
# SRC_CC, SRC_CPP or SRC_C
!ifdef SRC_CC
ALLOBJS    = $(SRC_CC:.cc=.o)
!endif
!ifdef SRC_CPP
!ifdef ALLOBJS
ALLOBJS = $(ALLOBJS) $(SRC_CPP:.cpp=.o)
!else
ALLOBJS = $(SRC_CPP:.cpp=.o)
!endif
!endif
!ifdef SRC_C
!ifdef ALLOBJS
ALLOBJS = $(ALLOBJS) $(SRC_C:.c=.o)
!else
ALLOBJS = $(SRC_C:.c=.o)
!endif
!endif

!ifdef SRC_RC
ALLRESS = $(SRC_RC:.rc=.res)
#ALLRESS = $(ALLRESS:../=)
!endif
# ALLOBJS = $(ALLOBJS:somedir/=)

!if EXIST( .\nmake-src-prefix.mak )
# Include strip of path to sources, i.e. macro like
#   ALLOBJS = $(ALLOBJS:..\..\..\..\..\..\explore/../extern/boost/libs/test/src/=)
#   ALLOBJS = $(ALLOBJS:../=)
#   ALLRESS = $(ALLRESS:../=)
# Be careful about path spelling!
# Pay attention the order of this macro! THE ORDER IS SIGNIFICANT!
!include .\nmake-src-prefix.mak
!endif

ALLDEPS    = $(SRC_CC:.cc=.d) $(SRC_CPP:.cpp=.d) $(SRC_C:.c=.d)

#!if [echo ALLOBJS -$(ALLOBJS)-]
#!endif
# Following trick intended to add prefix
# set marker (spaces are significant here!):
OBJ=$(ALLOBJS:.o =.o@)
#!if [echo OBJ 1 -$(OBJ)-]
#!endif
# remove trailing marker (with white space):
#OBJ=$(OBJ:.o@ =.o)
# remove unwanted space as result of line extending, like
# target: dep1.cpp dep2.cpp \
#         dep3.cpp
# (note, that if write '... dep2.cpp\', no white space happens)
OBJ=$(OBJ:.o@ =.o@)
#!if [echo OBJ 2 -$(OBJ)-]
#!endif
# replace marker by prefix:
#OBJ=$(OBJ:.o@=.o %OUTPUT_DIR%/)
# sorry, but I still not know how substitute macros in braces ();
!if "$(COMPILER_NAME)" == "evc4-arm"
OBJ=$(OBJ:.o@=.o obj\evc4-arm\shared\)
!elseif "$(COMPILER_NAME)" == "evc4-x86"
OBJ=$(OBJ:.o@=.o obj\evc4-x86\shared\)
!elseif "$(COMPILER_NAME)" == "vc6"
OBJ=$(OBJ:.o@=.o obj\vc6\shared\)
!endif
#!if [echo OBJ 3 -$(OBJ)-]
#!endif
# add prefix to first element:
OBJ=$(OUTPUT_DIR)\$(OBJ)
#!if [echo -$(OBJ)-]
#!endif

# The same trick for OBJ_DBG:
OBJ_DBG=$(ALLOBJS:.o =.o@)
#OBJ_DBG=$(OBJ_DBG:.o@ =.o)
OBJ_DBG=$(OBJ_DBG:.o@ =.o@)
#OBJ=$(OBJ:.o@=.o %OUTPUT_DIR%/)
# sorry, but I still not know how substitute macros in braces ();
!if "$(COMPILER_NAME)" == "evc4-arm"
OBJ_DBG=$(OBJ_DBG:.o@=.o obj\evc4-arm\shared-g\)
!elseif "$(COMPILER_NAME)" == "evc4-x86"
OBJ_DBG=$(OBJ_DBG:.o@=.o obj\evc4-x86\shared-g\)
!elseif "$(COMPILER_NAME)" == "vc6"
OBJ_DBG=$(OBJ_DBG:.o@=.o obj\vc6\shared-g\)
!endif
# add prefix to first element:
OBJ_DBG=$(OUTPUT_DIR_DBG)\$(OBJ_DBG)

# And for OBJ_STLDBG too:
OBJ_STLDBG=$(ALLOBJS:.o =.o@)
#OBJ_STLDBG=$(OBJ_STLDBG:.o@ =.o)
OBJ_STLDBG=$(OBJ_STLDBG:.o@ =.o@)
#OBJ=$(OBJ:.o@=.o %OUTPUT_DIR%/)
# sorry, but I still not know how substitute macros in braces ();
!if "$(COMPILER_NAME)" == "evc4-arm"
OBJ_STLDBG=$(OBJ_STLDBG:.o@=.o obj\evc4-arm\shared-stlg\)
!elseif "$(COMPILER_NAME)" == "evc4-x86"
OBJ_STLDBG=$(OBJ_STLDBG:.o@=.o obj\evc4-x86\shared-stlg\)
!elseif "$(COMPILER_NAME)" == "vc6"
OBJ_STLDBG=$(OBJ_STLDBG:.o@=.o obj\vc6\shared-stlg\)
!endif
# add prefix to first element:
OBJ_STLDBG=$(OUTPUT_DIR_STLDBG)\$(OBJ_STLDBG)

OBJ_A=$(ALLOBJS:.o =.o@)
OBJ_A=$(OBJ_A:.o@ =.o@)
!if "$(COMPILER_NAME)" == "evc4-arm"
OBJ_A=$(OBJ_A:.o@=.o obj\evc4-arm\static\)
!elseif "$(COMPILER_NAME)" == "evc4-x86"
OBJ_A=$(OBJ_A:.o@=.o obj\evc4-x86\static\)
!elseif "$(COMPILER_NAME)" == "vc6"
OBJ_A=$(OBJ_A:.o@=.o obj\vc6\static\)
!endif
OBJ_A=$(OUTPUT_DIR_A)\$(OBJ_A)

OBJ_A_DBG=$(ALLOBJS:.o =.o@)
OBJ_A_DBG=$(OBJ_A_DBG:.o@ =.o@)
!if "$(COMPILER_NAME)" == "evc4-arm"
OBJ_A_DBG=$(OBJ_A_DBG:.o@=.o obj\evc4-arm\static-g\)
!elseif "$(COMPILER_NAME)" == "evc4-x86"
OBJ_A_DBG=$(OBJ_A_DBG:.o@=.o obj\evc4-x86\static-g\)
!elseif "$(COMPILER_NAME)" == "vc6"
OBJ_A_DBG=$(OBJ_A_DBG:.o@=.o obj\vc6\static-g\)
!endif
OBJ_A_DBG=$(OUTPUT_DIR_A_DBG)\$(OBJ_A_DBG)

OBJ_A_STLDBG=$(ALLOBJS:.o =.o@)
OBJ_A_STLDBG=$(OBJ_A_STLDBG:.o@ =.o@)
!if "$(COMPILER_NAME)" == "evc4-arm"
OBJ_A_STLDBG=$(OBJ_A_STLDBG:.o@=.o obj\evc4-arm\static-stlg\)
!elseif "$(COMPILER_NAME)" == "evc4-x86"
OBJ_A_STLDBG=$(OBJ_A_STLDBG:.o@=.o obj\evc4-x86\static-stlg\)
!elseif "$(COMPILER_NAME)" == "vc6"
OBJ_A_STLDBG=$(OBJ_A_STLDBG:.o@=.o obj\vc6\static-stlg\)
!endif
OBJ_A_STLDBG=$(OUTPUT_DIR_A_STLDBG)\$(OBJ_A_STLDBG)

!ifdef ALLRESS
RES=$(ALLRESS:.res =.res@)
RES=$(RES:.res@ =.res@)
!if "$(COMPILER_NAME)" == "evc4-arm"
RES=$(RES:.res@=.res obj\evc4-arm\shared\)
!elseif "$(COMPILER_NAME)" == "evc4-x86"
RES=$(RES:.res@=.res obj\evc4-x86\shared\)
!elseif "$(COMPILER_NAME)" == "vc6"
RES=$(RES:.res@=.res obj\vc6\shared\)
!endif
RES=$(OUTPUT_DIR)\$(RES)

RES_DBG=$(ALLRESS:.res =.res@)
RES_DBG=$(RES_DBG:.res@ =.res@)
!if "$(COMPILER_NAME)" == "evc4-arm"
RES_DBG=$(RES_DBG:.res@=.res obj\evc4-arm\shared-g\)
!elseif "$(COMPILER_NAME)" == "evc4-x86"
RES_DBG=$(RES_DBG:.res@=.res obj\evc4-x86\shared-g\)
!elseif "$(COMPILER_NAME)" == "vc6"
RES_DBG=$(RES_DBG:.res@=.res obj\vc6\shared-g\)
!endif
RES_DBG=$(OUTPUT_DIR_DBG)\$(RES_DBG)

RES_STLDBG=$(ALLRESS:.res =.res@)
RES_STLDBG=$(RES_STLDBG:.res@ =.res@)
!if "$(COMPILER_NAME)" == "evc4-arm"
RES_STLDBG=$(RES_STLDBG:.res@=.res obj\evc4-arm\shared-stlg\)
!elseif "$(COMPILER_NAME)" == "evc4-x86"
RES_STLDBG=$(RES_STLDBG:.res@=.res obj\evc4-x86\shared-stlg\)
!elseif "$(COMPILER_NAME)" == "vc6"
RES_STLDBG=$(RES_STLDBG:.res@=.res obj\vc6\shared-stlg\)
!endif
RES_STLDBG=$(OUTPUT_DIR_STLDBG)\$(RES_STLDBG)
!endif


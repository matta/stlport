# -*- makefile -*- Time-stamp: <03/10/26 23:11:03 ptr>
# $Id$

release-shared:	$(OUTPUT_DIR) $(PRG)

dbg-shared:	$(OUTPUT_DIR_DBG) $(PRG_DBG)

stldbg-shared:	$(OUTPUT_DIR_STLDBG) $(PRG_STLDBG)

$(PRG):	$(OBJ) $(LIBSDEP)
	$(LINK_cc_REL) $(LINK_OUTPUT_OPTION) $(OBJ) $(LDLIBS_REL) $(LDLIBS)

$(PRG_DBG):	$(OBJ_DBG) $(LIBSDEP)
	$(LINK_cc_DBG) $(LINK_OUTPUT_OPTION) $(OBJ_DBG) $(LDLIBS_DBG) $(LDLIBS)

$(PRG_STLDBG):	$(OBJ_STLDBG) $(LIBSDEP)
	$(LINK_cc_STLDBG) $(LINK_OUTPUT_OPTION) $(OBJ_STLDBG) $(LDLIBS_STLDBG) $(LDLIBS)


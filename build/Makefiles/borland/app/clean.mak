# -*- makefile -*- Time-stamp:
# $Id: clean.mak

clobber: clean

	@if exist $(PRG)          del $(PRG)
	@if exist $(PRG_DBG)      del $(PRG_DBG)
	@if exist $(PRG_STLDBG)   del $(PRG_STLDBG)
	@if exist $(PRG_A)        del $(PRG_A)
	@if exist $(PRG_A_DBG)    del $(PRG_A_DBG)
	@if exist $(PRG_A_STLDBG) del $(PRG_A_STLDBG)

	@if exist $(OUTPUT_DIR)          rd /S /Q $(OUTPUT_DIR)
	@if exist $(OUTPUT_DIR_DBG)      rd /S /Q $(OUTPUT_DIR_DBG)
	@if exist $(OUTPUT_DIR_STLDBG)   rd /S /Q $(OUTPUT_DIR_STLDBG)
	@if exist $(OUTPUT_DIR_A)        rd /S /Q $(OUTPUT_DIR_A)
	@if exist $(OUTPUT_DIR_A_DBG)    rd /S /Q $(OUTPUT_DIR_A_DBG)
	@if exist $(OUTPUT_DIR_A_STLDBG) rd /S /Q $(OUTPUT_DIR_A_STLDBG)

	@if exist obj\$(TARGET_NAME)$(COMPILER_NAME) rd /S /Q obj\$(TARGET_NAME)$(COMPILER_NAME)
	@if exist obj rd /S /Q obj

	@echo clobber done.

distclean:      clobber

	@if exist $(INSTALL_BIN_DIR)\$(PRG_NAME)        del $(INSTALL_BIN_DIR)\$(PRG_NAME)
	@if exist $(INSTALL_BIN_DIR)\$(PRG_NAME_DBG)    del $(INSTALL_BIN_DIR)\$(PRG_NAME_DBG)
	@if exist $(INSTALL_BIN_DIR)\$(PRG_NAME_STLDBG) del $(INSTALL_BIN_DIR)\$(PRG_NAME_STLDBG)

	@if exist $(INSTALL_BIN_DIR)\$(PRG_NAME_A)        del $(INSTALL_BIN_DIR)\$(PRG_NAME_A)
	@if exist $(INSTALL_BIN_DIR)\$(PRG_NAME_A_DBG)    del $(INSTALL_BIN_DIR)\$(PRG_NAME_A_DBG)
	@if exist $(INSTALL_BIN_DIR)\$(PRG_NAME_A_STLDBG) del $(INSTALL_BIN_DIR)\$(PRG_NAME_A_STLDBG)
	
	@echo distclean done.



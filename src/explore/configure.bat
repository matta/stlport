@ECHO OFF
REM **************************************************************************
REM *
REM * configure.bat for setting up compiling STLport under Windows
REM * to see available options, call with option --help
REM *
REM * Copyright (C) 2004 Michael Fink
REM *
REM **************************************************************************

REM Attention! Batch file labels only have 8 significant characters!

echo STLport Configuration Tool for Windows
echo.

REM no options at all?
if NOT "%1xyz123" == "xyz123" goto init

echo Please specify some options or use "configure.bat --help" to see the
echo available options.
goto skp_comp

:init

REM initially create/overwrite config.mak
echo # STLport Configuration Tool for Windows > .\Makefiles\config.mak
echo # >> .\Makefiles\config.mak
echo # config.mak generated with command line: >> .\Makefiles\config.mak
echo # configure.bat %1 %2 %3 %4 %5 %6 %7 %8 %9 >> .\Makefiles\config.mak
echo # >> .\Makefiles\config.mak

set STLPORT_COMPILE_COMMAND=

REM
REM option loop
REM
:loop

REM help option
if "%1" == "-?" goto opt_help
if "%1" == "-h" goto opt_help
if "%1" == "/?" goto opt_help
if "%1" == "/h" goto opt_help
if "%1" == "--help" goto opt_help

REM compiler option
if "%1" == "-c" goto opt_comp
if "%1" == "/c" goto opt_comp
if "%1" == "--compiler" goto opt_comp

REM processor option
if "%1" == "-p" goto opt_proc
if "%1" == "/p" goto opt_proc
if "%1" == "--processor" goto opt_proc

REM cross compiling
if "%1" == "-x" goto opt_x
if "%1" == "/x" goto opt_x
if "%1" == "--cross" goto opt_x


echo Unknown option: %1

:cont_lp

shift

REM no more options?
if "%1xyz123" == "xyz123" goto end_loop

goto loop


REM **************************************************************************
REM *
REM * Help
REM *
REM **************************************************************************
:opt_help
echo The following options are available:
echo.
echo "-c <compiler>" or "--compiler <compiler>"
echo    Uses specified compiler to compile STLport. The following keywords
echo    are available:
echo    msvc6    Microsoft Visual C++ 6.0
echo    msvc7    Microsoft Visual C++ .NET 2002
echo    msvc71   Microsoft Visual C++ .NET 2003
echo    msvc8    Microsoft Visual C++ .NET 2005 (beta)
REM echo    icl      Intel C++ Compiler
echo    evc3     Microsoft eMbedded Visual C++ 3
echo    evc4     Microsoft eMbedded Visual C++ .NET
echo.
echo "-p <processor>" or "--processor <processor>"
echo    Sets target processor for given compiler; currently only used for
echo    evc3 and evc4 compiler. The following keywords are available:
echo    ARM     ARM processor
echo    x86     x86 processor (Emulator)
echo.
echo "-x"
echo    Enables cross-compiling; the result is that all built files that are
echo    normally put under "bin" and "lib" get extra subfolders depending on
echo    the compiler name.
goto skp_comp

REM **************************************************************************
REM *
REM * Compiler configuration
REM *
REM **************************************************************************
:opt_comp

set STLPORT_SELECTED_PROC=%2

if "%2" == "msvc6" goto oc_msvc6
if "%2" == "msvc7" goto oc_msvc7
if "%2" == "msvc71" goto oc_msvc71
if "%2" == "msvc8" goto oc_msvc8
if "%2" == "icl"   goto oc_icl

if "%2" == "evc3" goto oc_evc3
if "%2" == "evc4" goto oc_evc4

echo Unknown compiler: %2
goto oc_end

:oc_msvc6
echo Setting compiler: Microsoft Visual C++ 6.0
echo TARGET_OS=x86 >> .\Makefiles\config.mak
set STLPORT_COMPILE_COMMAND=nmake -f nmake-vc6.mak
goto oc_end

:oc_msvc7
echo Setting compiler: Microsoft Visual C++ .NET 2002
echo TARGET_OS=x86 >> .\Makefiles\config.mak
set STLPORT_COMPILE_COMMAND=nmake -f nmake-vc70.mak
goto oc_end

:oc_msvc71
echo. > .\Makefiles\config.mak
echo Setting compiler: Microsoft Visual C++ .NET 2003
echo TARGET_OS=x86 >> .\Makefiles\config.mak
set STLPORT_COMPILE_COMMAND=nmake -f nmake-vc71.mak
goto oc_end

:oc_msvc8
echo. > .\Makefiles\config.mak
echo Setting compiler: Microsoft Visual C++ .NET 2005 (beta)
echo TARGET_OS=x86 >> .\Makefiles\config.mak
set STLPORT_COMPILE_COMMAND=nmake -f nmake-vc8.mak
goto oc_end

:oc_icl
echo Compiler not supported in "explore" yet: Intel C++ Compiler
REM echo Setting compiler: Intel C++ Compiler
REM set STLPORT_COMPILE_COMMAND=nmake -f nmake-icl.mak
goto oc_end

:oc_evc3
echo Setting compiler: Microsoft eMbedded Visual C++ 3
echo COMPILER_NAME=evc3 >> .\Makefiles\config.mak
echo CEVERSION=300 >> .\Makefiles\config.mak
set STLPORT_COMPILE_COMMAND=nmake -f nmake-evc3.mak
goto oc_end

:oc_evc4
echo Setting compiler: Microsoft eMbedded Visual C++ .NET
echo COMPILER_NAME=evc4 >> .\Makefiles\config.mak
echo CEVERSION=420 >> .\Makefiles\config.mak
set STLPORT_COMPILE_COMMAND=nmake -f nmake-evc4.mak
goto oc_end

:oc_end
shift

goto cont_lp


REM **************************************************************************
REM *
REM * Target processor configuration
REM *
REM **************************************************************************
:opt_proc

if "%STLPORT_SELECTED_PROC%" == "evc3" goto op_ok
if "%STLPORT_SELECTED_PROC%" == "evc4" goto op_ok

echo Error: Setting processor for compiler other than evc3 and evc4!
goto op_end

:op_ok

if "%2" == "ARM" goto op_arm
if "%2" == "Arm" goto op_arm
if "%2" == "arm" goto op_arm

if "%2" == "X86" goto op_x86
if "%2" == "x86" goto op_x86

echo Unknown processor: %2
goto op_end

:op_arm
echo Setting processor: ARM
echo TARGET_PROC=arm >> .\Makefiles\config.mak
goto op_end

:op_x86
echo Setting processor: x86 (Emulator)
echo TARGET_PROC=x86 >> .\Makefiles\config.mak
goto op_end

:op_end
shift

goto cont_lp


REM **************************************************************************
REM *
REM * Cross Compiling option
REM *
REM **************************************************************************

:opt_x
echo Setting up for cross compiling.
echo CROSS_COMPILING=1 >> .\Makefiles\config.mak
goto cont_lp


REM **************************************************************************
REM *
REM * End loop
REM *
REM **************************************************************************

:end_loop

echo Done configuring STLport.
echo.

if "%STLPORT_COMPILE_COMMAND%" == "" goto skp_comp
echo Please type "%STLPORT_COMPILE_COMMAND%" to build STLport.
echo Type "%STLPORT_COMPILE_COMMAND% install" to install STLport to the "lib"
echo and "bin" folder when done.
echo.

:skp_comp
set STLPORT_SELECTED_PROC=
set STLPORT_COMPILE_COMMAND=

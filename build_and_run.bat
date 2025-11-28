@echo off
REM Build and run script for items.dat-decoder
REM Usage:
REM   build_and_run.bat                - builds and runs without args
REM   build_and_run.bat --json         - builds and runs and writes items.json
REM   build_and_run.bat --json out.json - writes to out.json

SETLOCAL
SET ARGS=%*
echo Building project...
call build.bat
if errorlevel 1 (
	echo Build failed.
	exit /b 1
)
echo Build succeeded.

echo Running itemsdat_decoder.exe %ARGS%

REM If no args, just run
if "%ARGS%"=="" (
	.\itemsdat_decoder.exe
	exit /b %ERRORLEVEL%
)

REM If exactly --json, run and let program choose default items.json
if "%ARGS%"=="--json" (
	.\itemsdat_decoder.exe --json
	exit /b %ERRORLEVEL%
)

REM Otherwise forward all args (supports --json out.json etc.)
.\itemsdat_decoder.exe %ARGS%
exit /b %ERRORLEVEL%
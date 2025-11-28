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

if "%ARGS%"=="" (
	.\itemsdat_decoder.exe --json-stdout > items.json
	exit /b %ERRORLEVEL%
)

.\itemsdat_decoder.exe %ARGS%
exit /b %ERRORLEVEL%
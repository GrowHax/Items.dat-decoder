@echo off
echo Compiling ExtendBuffer.c...
gcc -Iinclude -Wall -Wextra -c src/ExtendBuffer.c -o src/ExtendBuffer.o
if %errorlevel% neq 0 exit /b %errorlevel%

echo Compiling file_io.c...
gcc -Iinclude -Wall -Wextra -c src/file_io.c -o src/file_io.o
if %errorlevel% neq 0 exit /b %errorlevel%

echo Compiling ItemDefinition.c...
gcc -Iinclude -Wall -Wextra -c src/ItemDefinition.c -o src/ItemDefinition.o
if %errorlevel% neq 0 exit /b %errorlevel%

echo Compiling ItemsDat.c...
gcc -Iinclude -Wall -Wextra -c src/ItemsDat.c -o src/ItemsDat.o
if %errorlevel% neq 0 exit /b %errorlevel%

echo Compiling main.c...
gcc -Iinclude -Wall -Wextra -c main.c -o main.o
if %errorlevel% neq 0 exit /b %errorlevel%

echo Linking...
gcc main.o src/ExtendBuffer.o src/file_io.o src/ItemDefinition.o src/ItemsDat.o -o itemsdat_decoder.exe
if %errorlevel% neq 0 exit /b %errorlevel%

echo Build successful!

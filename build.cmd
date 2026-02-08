@echo off
setlocal enabledelayedexpansion

:: Specifically for speed
set CFLAGS=-O3 -march=native -flto -fno-exceptions -fno-rtti ^
-ffast-math -funroll-loops -ftree-vectorize -mavx2 -s -pipe ^
-fno-math-errno -DNDEBUG -Wall -Wextra ^
-ffunction-sections -fdata-sections -Wl,--gc-sections

set INCLUDES=-Iinc -Ilib
set LIBS=-lopengl32 -lgdi32 -luser32 -lole32

set SOURCES=main.cpp src\*.cpp

g++ -std=c++20 %CFLAGS% %INCLUDES% %SOURCES% %LIBS% -o tori.exe

if %errorlevel% equ 0 (
    echo Build succeeded
    tori.exe
) else (
    echo Build failed
    exit /b 1
)
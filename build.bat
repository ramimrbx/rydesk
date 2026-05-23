@echo off
echo ===================================================
echo             RyDesk CLI Build System
echo ===================================================
echo.

:: 1. Compile resource file using windres
set RES_FILE=
if exist "resources\rydesk.rc" (
    echo [RyDesk] Found resource file at: resources\rydesk.rc
    echo [RyDesk] Compiling resource file...
    windres "resources\rydesk.rc" -o rydesk_res.o
    
    if exist rydesk_res.o (
        set RES_FILE=rydesk_res.o
    ) else (
        echo [Warning] windres failed to compile the resource file. Check icon path inside .rc file.
    )
) else (
    echo [RyDesk] Notice: resources\rydesk.rc not found. Compiling without custom icon.
)

:: 2. Locate the main C++ source file
set SRC_FILE=
if exist rydesk.cpp (
    set SRC_FILE=rydesk.cpp
) else if exist main.cpp (
    set SRC_FILE=main.cpp
)

if "%SRC_FILE%"=="" (
    echo [Error] Neither rydesk.cpp nor main.cpp was found in the root directory!
    goto end
)

:: 3. Final compilation with G++
echo [RyDesk] Compiling %SRC_FILE% using C++20...
g++ %SRC_FILE% %RES_FILE% -std=c++20 -o rydesk.exe

if %ERRORLEVEL% equ 0 (
    echo.
    echo ===================================================
    echo [Success] rydesk.exe created successfully!
    echo ===================================================
    
    :: Cleanup the temporary object file to keep directory clean
    if exist rydesk_res.o del rydesk_res.o
) else (
    echo.
    echo [Error] Compilation failed!
)

:end
pause
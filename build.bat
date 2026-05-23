@echo off
echo ===================================================
echo             RyDesk CLI Build System
echo ===================================================
echo.

:: 1. Determine Build Mode from Command Line Argument
set BUILD_MODE=
if /I "%1"=="debug" (
    set BUILD_MODE=debug
) else if /I "%1"=="publish" (
    set BUILD_MODE=release
) else if "%1"=="" (
    echo [Notice] No argument provided. Defaulting to 'publish' mode...
    set BUILD_MODE=release
) else (
    echo [Error] Invalid argument: "%1"
    echo Usage: build.bat [debug ^| publish]
    goto end
)

:: 2. Create build directories if they don't exist
if not exist "build\debug" mkdir "build\debug"
if not exist "build\release" mkdir "build\release"

:: 3. Compile Resource File (If exists)
set RC_PATH=src\main\resources\rydesk.rc
set RES_FILE=

if exist "%RC_PATH%" (
    echo [RyDesk] Compiling resource file...
    windres "%RC_PATH%" -o build\rydesk_res.o
    set RES_FILE=build\rydesk_res.o
) else (
    echo [Notice] rydesk.rc not found in src\main\resources.
)

:: 4. Setup Source Directories
set SRC_DIR=src\main\cpp\com\rybecx\rydesk
set INC_DIR=src\main\cpp\com\rybecx\rydesk\include

if not exist "%SRC_DIR%\RydeskApp.cpp" (
    echo [Error] RydeskApp.cpp not found at %SRC_DIR%!
    goto end
)

:: 5. Compile everything together
echo [RyDesk] Compiling RyDesk CLI Engine (%BUILD_MODE% mode)...

if "%BUILD_MODE%"=="debug" (
    g++ "%SRC_DIR%\RydeskApp.cpp" %RES_FILE% -I "%INC_DIR%" -std=c++20 -g -o "build\debug\rydesk_debug.exe"
    set OUT_FILE=build\debug\rydesk_debug.exe
) else (
    g++ "%SRC_DIR%\RydeskApp.cpp" %RES_FILE% -I "%INC_DIR%" -std=c++20 -O3 -o "build\release\rydesk.exe"
    set OUT_FILE=build\release\rydesk.exe
)

if %ERRORLEVEL% equ 0 (
    echo.
    echo ===================================================
    echo [Success] %OUT_FILE% created successfully!
    echo ===================================================
    :: Cleanup temp object file
    if exist build\rydesk_res.o del build\rydesk_res.o
) else (
    echo.
    echo [Error] Compilation failed! Please check your C++ code.
)

:end
pause
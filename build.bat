@echo off
setlocal

set "PROJECT_DIR=%~dp0"
set "DEBUG_DIR=%PROJECT_DIR%Debug"
set "MAKE_PATH=D:\ST\STM32CubeIDE_1.19.0\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.make.win32_2.2.0.202409170845\tools\bin"
set "GNU_TOOLS_PATH=D:\ST\STM32CubeIDE_1.19.0\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.13.3.rel1.win32_1.0.0.202411081344\tools\bin"

set "PATH=%GNU_TOOLS_PATH%;%MAKE_PATH%;%PATH%"

if not exist "%DEBUG_DIR%" (
    echo [ERROR] Debug directory not found: %DEBUG_DIR%
    exit /b 1
)

pushd "%DEBUG_DIR%" >nul

echo ==================================================
echo STM32 Project Build Script
echo ==================================================
echo Project: PFLIGHT_26
echo Debug Dir: %DEBUG_DIR%
echo Toolchain: GNU Tools for STM32 v13.3
echo ==================================================
echo(

if /I "%~1"=="-c" (
    echo [CLEAN] Removing build artifacts...
    make clean
    set "EXIT_CODE=%ERRORLEVEL%"
    popd >nul
    exit /b %EXIT_CODE%
)

if /I "%~1"=="--clean-only" (
    echo [CLEAN] Removing build artifacts...
    make clean
    set "EXIT_CODE=%ERRORLEVEL%"
    popd >nul
    exit /b %EXIT_CODE%
)

echo [BUILD] Compiling...

if /I "%~1"=="--no-clean" (
    echo [INFO] Skipping clean...
) else (
    echo [CLEAN] Removing old build artifacts...
    make clean
    if errorlevel 1 (
        set "EXIT_CODE=%ERRORLEVEL%"
        popd >nul
        exit /b %EXIT_CODE%
    )
)

make all -j24
set "EXIT_CODE=%ERRORLEVEL%"

if "%EXIT_CODE%"=="0" (
    if exist "PFLIGHT_26.elf" (
        echo(
        echo ==================================================
        echo Build succeeded!
        echo ==================================================
        echo(
        echo Output files:
        echo   - PFLIGHT_26.elf  (Executable)
        echo   - PFLIGHT_26.map  (Memory map)
        echo   - PFLIGHT_26.list (Disassembly)
        echo(
        echo Binary size:
        arm-none-eabi-size PFLIGHT_26.elf
    ) else (
        set "EXIT_CODE=1"
        echo(
        echo ==================================================
        echo Build failed!
        echo ==================================================
    )
) else (
    echo(
    echo ==================================================
    echo Build failed!
    echo ==================================================
)

popd >nul
exit /b %EXIT_CODE%
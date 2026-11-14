@echo off
echo ========================================================
echo  DSA Visualizer Studio - Automated Build Script (C++17)
echo ========================================================
echo.

set PATH=C:\msys64\ucrt64\bin;C:\Users\%USERNAME%\AppData\Local\Programs\Python\Python312\Scripts;%PATH%

echo [1/3] Checking Toolchain...
gcc --version >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo [ERROR] GCC compiler not found in C:\msys64\ucrt64\bin!
    pause
    exit /b 1
)

cmake --version >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo [ERROR] CMake not found in PATH!
    pause
    exit /b 1
)

echo [2/3] Configuring CMake with Ninja generator...
cmake -B "build" -S "." -G "Ninja" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Release
if %ERRORLEVEL% neq 0 (
    echo [ERROR] CMake configuration failed!
    pause
    exit /b 1
)

echo.
echo [3/3] Compiling Project (dsa_studio and test_dsa)...
cmake --build "build" --config Release
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Build failed! Check compiler logs.
    pause
    exit /b 1
)

echo.
echo ========================================================
echo  BUILD SUCCESSFUL!
echo  Executable: build\dsa_studio.exe
echo  Test Suite: build\test_dsa.exe
echo ========================================================
pause

@echo off
set PATH=C:\msys64\ucrt64\bin;%PATH%
if exist "build\test_dsa.exe" (
    echo ========================================
    echo  Running Unit Test Suite
    echo ========================================
    "build\test_dsa.exe"
    echo.
    echo ========================================
    echo  Running CTest
    echo ========================================
    ctest --test-dir "build" --output-on-failure
    pause
) else (
    echo [ERROR] Test binary not found! Please run build.bat first.
    pause
)

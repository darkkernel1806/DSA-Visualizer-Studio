@echo off
set PATH=C:\msys64\ucrt64\bin;%PATH%
if exist "build\dsa_studio.exe" (
    echo Starting DSA Visualizer Studio...
    start "" "build\dsa_studio.exe"
) else (
    echo [ERROR] Application binary not found! Please run build.bat first.
    pause
)

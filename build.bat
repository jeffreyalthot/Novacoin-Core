@echo off
REM Build Novacoin-Core on Windows using CMake and Visual Studio
setlocal
if not exist build mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 ..
if errorlevel 1 exit /b %errorlevel%
cmake --build . --config Release
if errorlevel 1 exit /b %errorlevel%
echo Build complete. Executable is in build\Release\
@echo off
cmake -S . -B build
if %ERRORLEVEL% == 1 exit
cmake --build ./build
if %ERRORLEVEL% == 1 exit

@echo off
if not exist build exit
cmake --build ./build --target clean
rd /s /q build
@echo off
call build.bat
echo ________________________
echo. && echo Running... && echo.
set path=build\Debug
if /I "%1"=="release" set path=build\Release

for %%f in (%path%\*.exe) do (
	if "%%~xf"==".exe" %%f
)

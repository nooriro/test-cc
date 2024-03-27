@pushd %~dp0
@setlocal
@for %%x in (
    "%ProgramFiles(x86)%\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
    "%ProgramFiles%\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
    "%ProgramFiles%\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat"
    "%ProgramFiles%\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
) do @if exist %%x set "VCVARS64=%%x" & goto :found
@>&2 echo Please install Visual Studio Build Tools 2022 or Visual Studio 2022
@>&2 echo and run this again.
@endlocal
@popd
@exit /b 1
:found
@REM Checking if a folder exists using a .bat file
@REM https://stackoverflow.com/questions/21033801/checking-if-a-folder-exists-using-a-bat-file
@set "DIR=out"
@if not exist %DIR%\ md %DIR% || exit /b 1  
call %VCVARS64% & echo on
cl /O2 /Brepro /utf-8 edivbug1.c /link /DEFAULTLIB:ucrt.lib /NODEFAULTLIB:libucrt.lib /Brepro /OUT:%DIR%\edivbug1.exe
cl /O2 /Brepro /utf-8 edivbug2.c /link /DEFAULTLIB:ucrt.lib /NODEFAULTLIB:libucrt.lib /Brepro /OUT:%DIR%\edivbug2.exe
cl /O2 /Brepro /utf-8 edivbug3.c /link /DEFAULTLIB:ucrt.lib /NODEFAULTLIB:libucrt.lib /Brepro /OUT:%DIR%\edivbug3.exe
@REM Edit/Replace text within a Variable
@REM https://ss64.com/nt/syntax-replace.html
call %VCVARS64:vcvars64=vcvars32% & echo on
cl /O2 /Brepro /utf-8 edivbug1.c /link /DEFAULTLIB:ucrt.lib /NODEFAULTLIB:libucrt.lib /Brepro /OUT:%DIR%\edivbug1_x86.exe
cl /O2 /Brepro /utf-8 edivbug2.c /link /DEFAULTLIB:ucrt.lib /NODEFAULTLIB:libucrt.lib /Brepro /OUT:%DIR%\edivbug2_x86.exe
cl /O2 /Brepro /utf-8 edivbug3.c /link /DEFAULTLIB:ucrt.lib /NODEFAULTLIB:libucrt.lib /Brepro /OUT:%DIR%\edivbug3_x86.exe
@echo.
@endlocal
@popd

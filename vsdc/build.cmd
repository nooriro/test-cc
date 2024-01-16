@pushd %~dp0
@setlocal
@REM call "%ProgramFiles%\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" x64 & echo on
@REM call "%ProgramFiles%\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat" x64 & echo on
@REM call "%ProgramFiles%\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat" x64 & echo on
call "%ProgramFiles(x86)%\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat" x64 & echo on
rc manifest.rc
cl /O2 /utf-8 /Brepro nonbmp.c /link manifest.res /Brepro
@endlocal
@popd

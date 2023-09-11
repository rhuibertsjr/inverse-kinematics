@echo off

if not exist ".\bin\" mkdir ".\bin"

pushd .\bin

rem Windows compilers yikes.
cl -Zi ..\src\core.c ..\src\math.c

popd .\bin

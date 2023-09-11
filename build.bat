@echo off

if not exist "S:\motion-and-vision\inverse-kinematics\bin\" mkdir "S:\motion-and-vision\inverse-kinematics\bin"

pushd .\bin

rem Windows compilers yikes.
cl -Zi ..\src\core.c ..\src\math.c

popd .\bin

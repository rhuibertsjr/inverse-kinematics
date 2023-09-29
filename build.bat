@echo off

if not exist ".\bin\" mkdir ".\bin"

pushd .\bin

if "%1"=="--no-raylib" (
cl /Zi ..\src\core.c ..\src\math.c 
) else (
cl /Zi /MD /DRAYLIB ..\src\core.c ..\src\math.c /link kernel32.lib opengl32.lib gdi32.lib winmm.lib ..\lib\raylib.lib user32.lib shell32.lib 
)

popd .\bin

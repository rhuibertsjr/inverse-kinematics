# Inverse Kinematics (CCD)

## 1. Prerequisite 
If you want to build this project from source, then you need the following dependencies:

- A working [MSVC](https://visualstudio.microsoft.com/vs/features/cplusplus/) C/C++ compiler, or the latest [GCC](https://gcc.gnu.org/) compiler.
- To visualize the CCD algorithm, you need [Raylib](https://www.raylib.com/).

> **Note**: While a building alternative for the [MSVC](https://visualstudio.microsoft.com/vs/features/cplusplus/) C/C++ compiler is provided, I 
> strongly recommend using the [MSVC](https://visualstudio.microsoft.com/vs/features/cplusplus/) compiler for easier integration.

### 1.1 Installing RayLib
On Windows 11 (Command Prompt), get precompiled binaries (included in the `lib/` folder 
```batchfile
$ git clone https://github.com/rhuibertsjr/inverse-kinematics.git
```
On Windows 11 (MinGW64/Msys2)
```bash
$ pacman -S mingw-w64-x86_64-raylib
```

## 2. Building
On Windows 11 (Command Prompt)
```batchfile
$ build
```
On Windows 11 (MinGW64)
```bash
$ ./build.sh
```

### 2.1 Building without dependencies
On Windows 11 (Command Prompt)
```batchfile
$ build --no-raylib
```
On Windows 11 (MinGW64)
```bash
$ ./build.sh --no-raylib
```

## 3. Getting started
On Windows 11 (Command Prompt)
```batchfile
$ .\bin\core.exe
```
On Windows 11 (MinGW64)
```bash
$ ./bin/myprogram.exe
```
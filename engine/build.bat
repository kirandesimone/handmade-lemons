@echo OFF
setlocal enabledelayedexpansion

set cFilenames=
for /R %%f in (*.c) do (
    set cFilenames=!cFilenames! %%f
)

set compilerFlags=-g -std=c17
set includeFlags=-Isrc
set linkerFlags=-luser32
set defines=-D_CRT_SECURE_NO_WARNINGS

mkdir ..\bin

echo "Building Engine..."
clang %cFilenames% %compilerFlags% -o ..\bin\engine.exe %defines% %includeFlags% %linkerFlags%
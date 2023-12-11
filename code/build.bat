@echo off

REM include .lib files
set Libraries64=

REM include folders
set Includes=/IC:\OBJtoRMF\code\include

REM Z7 generates a .pdb
set CommonCompilerFlags=/nologo /Z7

REM make a dir for the build
IF NOT EXIST ..\build mkdir ..\build
pushd ..\build

REM 64-bit build
cl %CommonCompilerFlags% ..\code\main.cpp %Includes% /link%Libraries64%  /subsystem:console
popd
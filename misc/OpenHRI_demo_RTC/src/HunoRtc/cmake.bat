echo off
rem 
rem  Batch File for RT Component Building
rem 
set PROJECT=Huno

set CMAKE_DIR=C:\Program Files (x86)\CMake 2.8
set VC10_DIR=C:\Program Files (x86)\Microsoft Visual Studio 10.0

if not exist "%CMAKE_DIR%" set CMAKE_DIR=C:\Program Files\CMake 2.8
if not exist "%VC10_DIR%" set VC10_DIR=C:\Program Files\Microsoft Visual Studio 10.0

echo on

mkdir build
cd build

"%CMAKE_DIR%\bin\cmake.exe" -G "Visual Studio 10" ..



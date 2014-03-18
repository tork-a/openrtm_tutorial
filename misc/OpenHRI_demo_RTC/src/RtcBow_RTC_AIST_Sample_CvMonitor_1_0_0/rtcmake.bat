echo off
rem 
rem  Batch File for RT Component Building
rem 

set CMAKE_DIR=C:\Program Files (x86)\CMake 2.8
set VC10_DIR=C:\Program Files (x86)\Microsoft Visual Studio 10.0

if not exist "%CMAKE_DIR%" set CMAKE_DIR=C:\Program Files\CMake 2.8
if not exist "%VC10_DIR%" set VC10_DIR=C:\Program Files\Microsoft Visual Studio 10.0

echo on

mkdir build
cd build

"%CMAKE_DIR%\bin\cmake.exe" -G "Visual Studio 10" ..
if exist "%VC10_DIR%\Common7\IDE\VCExpress.exe" "%VC10_DIR%\Common7\IDE\VCExpress.exe" CvMonitor.sln /Build Release
if exist "%VC10_DIR%\Common7\IDE\devenv.com" "%VC10_DIR%\Common7\IDE\devenv.com" CvMonitor.sln /Build Release

mkdir ..\rtc
copy Release\* ..\rtc\*
cd ..
copy .\*.conf .\rtc


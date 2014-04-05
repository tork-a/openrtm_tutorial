REM ''' Usage of this file
REM %1 usb drive to be used
REM '''

set usbdrive=%1
%usbdrive%
cd openrtm_tutorial\demo\BatchFiles
echo Moved to DIR=%cd%

tasklist |findstr "CommandInComp.exe" > nul

if %ERRORLEVEL% == 0  goto endproc

rem start .\CvMonitor.lnk
cd ..\GROBOT_vs10\bin
start .\CommandInComp.exe

cd ..\..\BatchFiles

goto end

:endproc
echo "CommandInComp.exe is already up"

:end

REM ''' Usage of this file
REM %1 usb drive to be used
REM '''

set usbdrive=%1
%usbdrive%
cd demo\BatchFiles
echo Moved to DIR=%cd%

tasklist |findstr "PowerPointServer.exe" > nul
rem tasklist |findstr "PPTVIEW.EXE" > nul :: 2/2/2014 PPTVIEWER was tried but no luck.

if %ERRORLEVEL% == 0  goto endproc

cd ..\Apps
start .\PowerPointServer.exe .\pps.conf
cd ..\BatchFiles

goto end

:endproc
echo "PowerPointServer is already up"

:end

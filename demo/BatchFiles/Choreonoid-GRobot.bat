REM ''' Usage of this file
REM %1 usb drive to be used
REM '''

set usbdrive=%1
%usbdrive%
cd openrtm_tutorial\demo\BatchFiles
echo Moved to DIR=%cd%

tasklist |findstr "choreonoid.exe" > nul

if %ERRORLEVEL% == 0  goto endproc

cd ..\Choreonoid-1.1
start .\bin\choreonoid.exe .\share\projects\GR001.cnoid
cd ..\BatchFiles

goto end

:endproc
echo "choreonoid is already up"


REM ''' Usage of this file
REM %1 usb drive to be used
REM '''

set usbdrive=%1
%usbdrive%
cd demo\MotionByVoiceDemo
echo Moved to DIR=%cd%

tasklist |findstr "choreonoid.exe" > nul

if %ERRORLEVEL% == 0  goto endproc

cd ..\Choreonoid-1.1
start .\bin\choreonoid.exe .\share\projects\GR001Sample2.cnoid
cd ..\MotionByVoiceDemo

goto end

:endproc
echo "choreonoid is already up"


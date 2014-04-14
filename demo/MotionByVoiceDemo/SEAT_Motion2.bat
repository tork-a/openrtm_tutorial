rem Adding another state to motion_cnoid.seatml

REM ''' Usage of this file
REM %1 usb drive to be used
REM '''

set usbdrive=%1
%usbdrive%
cd openrtm_tutorial\demo\MotionByVoiceDemo
echo Moved to DIR=%cd%

tasklist |findstr "SEAT.exe" > nul

if %ERRORLEVEL% == 0  goto endproc

start ..\SEATSAT\SEAT.exe motion_cnoid2.seatml

goto end

:endproc
echo "SEAT is already up"

:end

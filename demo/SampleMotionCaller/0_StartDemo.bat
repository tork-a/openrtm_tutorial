REM ''' Usage of this file
REM %1 usb drive to be used
REM '''

set usbdrive=%1
%usbdrive%
cd demo\SampleMotionCaller
echo Moved to DIR=%cd%

call ..\SampleMotionCaller\Choreonoid-GRobot_withMotion.bat

call ..\SampleMotionCaller\build\src\Debug\SampleMotionCallerComp.exe

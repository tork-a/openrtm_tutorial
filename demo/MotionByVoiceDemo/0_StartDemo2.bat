rem Adding another state

REM ''' Usage of this file
REM %1 usb drive to be used
REM '''

set usbdrive=%1
%usbdrive%
cd openrtm_tutorial\demo\MotionByVoiceDemo
echo Moved to DIR=%cd%

start ..\OpenHRIAudio\portaudioinput.exe
call Julius_Motion2.bat
call SEAT_Motion2.bat
call Choreonoid-GRobot_withMotion.bat

rem For debug
start ..\OpenHRIAudio\portaudiooutput.exe
rem call ..\SampleWordLogger\build\Debug\SampleWordLoggerComp.exe

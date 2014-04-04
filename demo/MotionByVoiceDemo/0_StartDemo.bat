REM ''' Usage of this file
REM %1 usb drive to be used
REM '''

set usbdrive=%1
%usbdrive%
cd openrtm_tutorial\demo\MotionByVoiceDemo
echo Moved to DIR=%cd%

echo CURRENT DIR=%cd%
set usbdrive=%2
if NOT DEFINED usbdrive set usbdrive=F:
cd /F %usbdrive%\   REM Changing directory to the USB drive.

rem These processes are added by trial to mimic "0 SpeechDemo"
start ..\OpenHRIAudio\portaudioinput.exe
call ..\MotionByVoiceDemo\Julius_Motion.bat
call ..\MotionByVoiceDemo\SEAT_Motion.bat
call ..\MotionByVoiceDemo\Choreonoid-GRobot_withMotion.bat

rem For debug
rem start ..\OpenHRIAudio\portaudiooutput.exe
rem call ..\SampleWordLogger\build\Debug\SampleWordLoggerComp.exe

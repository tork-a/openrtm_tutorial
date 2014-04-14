REM ''' Usage of this file
REM %1 usb drive to be used
REM '''

set RTM_ROOT=%usbdrive%\openrtm_tutorial\misc\OpenRTM-aist\1.1
set OHRI_ROOT=%usbdrive%\openrtm_tutorial\demo\OpenHRIAudio
set PATH=%PATH%;%RTM_ROOT%\runtime;%OHRI_ROOT%;%OMNI_ROOT%\bin\x86_win32

set usbdrive=%1
%usbdrive%
cd openrtm_tutorial\demo\MotionByVoiceDemo
echo Moved to DIR=%cd%

rem These processes are added by trial to mimic "0 SpeechDemo"
start ..\OpenHRIAudio\portaudioinput.exe
call ..\MotionByVoiceDemo\Julius_Motion.bat
call ..\MotionByVoiceDemo\SEAT_Motion.bat
call ..\MotionByVoiceDemo\Choreonoid-GRobot_withMotion.bat

rem For debug
rem start ..\OpenHRIAudio\portaudiooutput.exe
rem call ..\SampleWordLogger\build\Debug\SampleWordLoggerComp.exe

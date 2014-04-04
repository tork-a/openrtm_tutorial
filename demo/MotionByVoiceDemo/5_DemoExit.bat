REM ''' Usage of this file
REM %1 usb drive to be used
REM '''

set usbdrive=%1
%usbdrive%
cd openrtm_tutorial\demo\MotionByVoiceDemo
echo Moved to DIR=%cd%

for /f "usebackq" %%h in (`hostname`) do set HOSTNAME=%%h

call ..\Apps\rtshell\rtexit 127.0.0.1/%HOSTNAME%.host_cxt/SEAT0.rtc

rem These processes are added by trial to mimic "0 SpeechDemo"
call ..\Apps\rtshell\rtexit 127.0.0.1/%HOSTNAME%.host_cxt/Choreonoid-GRobot0.rtc
call ..\Apps\rtshell\rtexit 127.0.0.1/%HOSTNAME%.host_cxt/JuliusRTC0.rtc
call ..\Apps\rtshell\rtexit 127.0.0.1/%HOSTNAME%.host_cxt/PortAudioInput0.rtc
taskkill /F /IM CHOREONOID.exe

rem For debug
call ..\Apps\rtshell\rtexit 127.0.0.1/%HOSTNAME%.host_cxt/PortAudioOutput0.rtc


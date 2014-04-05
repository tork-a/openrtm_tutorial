REM ''' Usage of this file
REM %1 usb drive to be used
REM '''

set usbdrive=%1
%usbdrive%
cd openrtm_tutorial\demo\MotionByVoiceDemo
echo Moved to DIR=%cd%

for /f "usebackq" %%h in (`hostname`) do set HOSTNAME=%%h

rem These processes are added by trial to mimic "0 SpeechDemo"
call ..\Apps\rtshell\rtcon 127.0.0.1/%HOSTNAME%.host_cxt/PortAudioInput0.rtc:AudioDataOut 127.0.0.1/%HOSTNAME%.host_cxt/JuliusRTC0.rtc:data
call ..\Apps\rtshell\rtcon 127.0.0.1/%HOSTNAME%.host_cxt/JuliusRTC0.rtc:result 127.0.0.1/%HOSTNAME%.host_cxt/SEAT0.rtc:speechin
call ..\Apps\rtshell\rtcon 127.0.0.1/%HOSTNAME%.host_cxt/SEAT0.rtc:command 127.0.0.1/%HOSTNAME%.host_cxt/Cnoid_RobotMotion0.rtc:command

rem For debug
rem call ..\Apps\rtshell\rtcon 127.0.0.1/%HOSTNAME%.host_cxt/JuliusRTC0.rtc:log 127.0.0.1/%HOSTNAME%.host_cxt/PortAudioOutput0.rtc:AudioDataIn

REM ''' Usage of this file
REM %1 usb drive to be used
REM '''

set usbdrive=%1
%usbdrive%
cd demo\MotionByVoiceLoggerDemo
echo Moved to DIR=%cd%

for /f "usebackq" %%h in (`hostname`) do set HOSTNAME=%%h

call "..\MotionByVoiceDemo\1_ConnectRTC.bat"

call ..\Apps\rtshell\rtcon 127.0.0.1/%HOSTNAME%.host_cxt/JuliusRTC0.rtc:result 127.0.0.1/%HOSTNAME%.host_cxt/SampleWordLogger0.rtc:result

REM ''' Usage of this file
REM %1 usb drive to be used
REM '''

set usbdrive=%1
%usbdrive%   REM Changing directory to the USB drive.
cd openrtm_tutorial\demo\0_SpeechDemo
echo Moved to DIR=%cd%

for /f "usebackq" %%h in (`hostname`) do set HOSTNAME=%%h

call ..\Apps\rtshell\rtcon 127.0.0.1/%HOSTNAME%.host_cxt/JuliusRTC0.rtc:data 127.0.0.1/%HOSTNAME%.host_cxt/PortAudioInput0.rtc:AudioDataOut
call ..\Apps\rtshell\rtcon 127.0.0.1/%HOSTNAME%.host_cxt/JuliusRTC0.rtc:result 127.0.0.1/%HOSTNAME%.host_cxt/SEAT0.rtc:gesture

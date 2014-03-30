echo CURRENT DIR=%cd%
set usbdrive=%1
if NOT DEFINED usbdrive set usbdrive=F:
cd /d %usbdrive%\

for /f "usebackq" %%h in (`hostname`) do set HOSTNAME=%%h

call ..\Apps\rtshell\rtexit 127.0.0.1/%HOSTNAME%.host_cxt/DirectShowCamComp0.rtc
call ..\Apps\rtshell\rtexit 127.0.0.1/%HOSTNAME%.host_cxt/Flip0.rtc
call ..\Apps\rtshell\rtexit 127.0.0.1/%HOSTNAME%.host_cxt/CameraViewerComp0.rtc

taskkill /F /IM DirectShowCamComp.exe
taskkill /F /IM FlipComp.exe
taskkill /F /IM CameraViewerComp.exe


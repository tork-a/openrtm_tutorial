REM ''' Usage of this file
REM %1 usb drive to be used
REM '''

set usbdrive=%1
%usbdrive%
cd demo\SampleMotionCaller
echo Moved to DIR=%cd%

for /f "usebackq" %%h in (`hostname`) do set HOSTNAME=%%h

call ..\Apps\rtshell\rtexit 127.0.0.1/%HOSTNAME%.host_cxt/Cnoid_RobotMotion0.rtc
call ..\Apps\rtshell\rtexit 127.0.0.1/%HOSTNAME%.host_cxt/SampleMotionCaller0.rtc

taskkill /F /IM SampleMotionCaller.exe
taskkill /F /IM CHOREONOID.exe


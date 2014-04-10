REM ''' Usage of this file
REM %1 usb drive to be used
REM '''

set usbdrive=%1
%usbdrive%
cd openrtm_tutorial\demo\SampleMotionCaller
echo Moved to DIR=%cd%

for /f "usebackq" %%h in (`hostname`) do set HOSTNAME=%%h


call ..\Apps\rtshell\rtdeact 127.0.0.1/%HOSTNAME%.host_cxt/Cnoid_RobotMotion0.rtc
call ..\Apps\rtshell\rtdeact 127.0.0.1/%HOSTNAME%.host_cxt/SampleMotionCaller0.rtc



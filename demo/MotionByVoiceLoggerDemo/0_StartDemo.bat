REM ''' Usage of this file
REM %1 usb drive to be used
REM '''

set usbdrive=%1
%usbdrive%
cd demo\MotionByVoiceLoggerDemo
echo Moved to DIR=%cd%

rem setup MotionByVoiceDemo first.
call ..\MotionByVoiceDemo\0_StartDemo.bat

rem setup SampleWordLogger
call ..\SampleWordLogger\0_StartDemo.bat
cd ..\MotionByVoiceLoggerDemo


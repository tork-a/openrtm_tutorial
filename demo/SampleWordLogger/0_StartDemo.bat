REM ''' Usage of this file
REM %1 usb drive to be used
REM '''

set usbdrive=%1
%usbdrive%
cd demo\SampleWordLogger
echo Moved to DIR=%cd%

rem setup SampleWordLogger
cd ..\SampleWordLogger\build\src\Debug
call SampleWordLoggerComp.exe
cd ..\..

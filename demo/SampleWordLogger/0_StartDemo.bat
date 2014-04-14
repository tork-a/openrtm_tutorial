REM ''' Usage of this file
REM %1 usb drive to be used
REM '''

set RTM_ROOT=%usbdrive%\openrtm_tutorial\misc\OpenRTM-aist\1.1
set PATH=%PATH%;%RTM_ROOT%\runtime;%RTM_ROOT%\bin;%RTM_ROOT%\omniORB\4.1.5\bin\x86_win32

set usbdrive=%1
%usbdrive%
cd openrtm_tutorial\demo\SampleWordLogger
echo Moved to DIR=%cd%

rem setup SampleWordLogger
cd ..\SampleWordLogger\build\src\Debug
call SampleWordLoggerComp.exe
cd ..\..

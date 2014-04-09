REM ''' Usage of this file
REM %1 usb drive to be used
REM '''

set usbdrive=%1
%usbdrive%
cd openrtm_tutorial\demo\SampleMotionCaller
echo Moved to DIR=%cd%

call ..\SampleMotionCaller\Choreonoid-GRobot_withMotion.bat

set RTM_ROOT=%usbdrive%\openrtm_tutorial\demo\OpenHRIAudio;%usbdrive%\openrtm_tutorial\misc\OpenRTM-aist\1.1\omniORB\4.1.5\bin\x86_win32;c:\Program Files (x86)\OpenRTM-aist\1.1\bin
set PATH=%PATH%;%RTM_ROOT%
start ..\SampleMotionCaller\build\src\Debug\SampleMotionCallerComp.exe

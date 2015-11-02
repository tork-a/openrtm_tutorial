echo CURRENT DIR=%cd%
set usbdrive=%1
if NOT DEFINED usbdrive set usbdrive=F:
cd /d %usbdrive%\

cd openrtm_tutorial/misc/VstoneScaraRobotRTC
set OMNI_ROOT=%usbdrive%\openrtm_tutorial\misc\OpenRTM-aist\1.1\omniORB\4.1.5
set RTM_ROOT=%usbdrive%\openrtm_tutorial\misc\OpenRTM-aist\1.1
rem set PATH=%PATH%;%OMNI_ROOT%\bin\x86_win32;%usbdrive%\openrtm_tutorial\misc\VstoneScaraRobotRTC
set PATH=%PATH%;%RTM_ROOT%\runtime;%OMNI_ROOT%\bin\x86_win32;%usbdrive%\misc\VstoneScaraRobotRTC

echo CURRENT DIR=%cd%
echo PATH=%PATH%

TIMEOUT /T 1
start vs_asr_rtccomp.exe
start scararobotcontrolrtccomp.exe

TIMEOUT /T 1

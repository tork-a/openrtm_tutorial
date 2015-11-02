echo CURRENT DIR=%cd%
set usbdrive=%1
if NOT DEFINED usbdrive set usbdrive=F:
cd /d %usbdrive%\

cd openrtm_tutorial/misc/OpenRTM-aist/1.1/examples/C++/opencv-rtcs
set TUTORIAL_ROOT=%usbdrive%\openrtm_tutorial
set OMNI_ROOT=%TUTORIAL_ROOT%\misc\OpenRTM-aist\1.1\omniORB\4.1.5
set RTM_ROOT=%TUTORIAL_ROOT%\misc\OpenRTM-aist\1.1
set OPENCV_ROOT=%TUTORIAL_ROOT%\demo\Rtc_CvMonitor
rem set PATH=%PATH%;%OMNI_ROOT%\bin\x86_win32;%TUTORIAL_ROOT%\misc\OpenRTM-aist\1.1\examples\C++\opencv-rtcs
set PATH=%PATH%;%RTM_ROOT%\runtime;%OPENCV_ROOT%;%OMNI_ROOT%\bin\x86_win32;%usbdrive%\misc\OpenRTM-aist\1.1\examples\C++\opencv-rtcs

echo CURRENT DIR=%cd%
echo PATH=%PATH%

TIMEOUT /T 1
start DirectShowCamComp.exe
start FlipComp.exe
start CameraViewerComp.exe

TIMEOUT /T 1

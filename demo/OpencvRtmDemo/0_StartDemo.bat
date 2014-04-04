echo CURRENT DIR=%cd%
set usbdrive=%1
if NOT DEFINED usbdrive set usbdrive=F:
cd /d %usbdrive%\

cd openrtm_tutorial/misc/OpenRTM-aist/1.1/examples/C++/opencv-rtcs
set OMNI_ROOT=%usbdrive%\openrtm_tutorial\misc\OpenRTM-aist\1.1\omniORB\4.1.5
set PATH=%PATH%;%OMNI_ROOT%\bin\x86_win32;%usbdrive%\openrtm_tutorial\misc\OpenRTM-aist\1.1\examples\C++\opencv-rtcs

echo CURRENT DIR=%cd%
echo PATH=%PATH%

TIMEOUT /T 1
start DirectShowCamComp.exe
start FlipComp.exe
start CameraViewerComp.exe

TIMEOUT /T 130
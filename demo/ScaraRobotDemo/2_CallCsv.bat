echo CURRENT DIR=%cd%
set usbdrive=%1
if NOT DEFINED usbdrive set usbdrive=F:
cd /d %usbdrive%\

cd openrtm_tutorial/misc/VstoneScaraRobotRTC

TIMEOUT /T 1

start Sample.csv

TIMEOUT /T 1

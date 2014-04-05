REM ''' Usage of this file
REM %1 usb drive to be used
REM '''

set usbdrive=%1
%usbdrive%   REM Changing directory to the USB drive.
cd openrtm_tutorial\demo  
echo Moved to DIR=%cd%

set JAVA_HOME=%usbdrive%\openrtm_tutorial\misc\openjdk_1.7.0_u45_2.4.3_installed
set PATH=;%usbdrive%\openrtm_tutorial\misc\openjdk_1.7.0_u45_2.4.3_installed\bin;%usbdrive%\openrtm_tutorial\misc\openjdk_1.7.0_u45_2.4.3_installed\jre\bin;%PATH%

cd %usbdrive%\openrtm_tutorial\misc\OpenRTM-aist\1.1\tools
RTSystemEditorRCP.exe

REM ''' Usage of this file
REM %1 usb drive to be used
REM '''

set usbdrive=%1
%usbdrive%
cd openrtm_tutorial\demo\BatchFiles
echo Moved to DIR=%cd%

tasklist |findstr "juliusrtc.exe" > nul

if %ERRORLEVEL% == 0  goto endproc

cd ..\OpenHRIVoice
start .\juliusrtc.exe ..\kinect_demo2.grxml
cd ..\BatchFiles

goto end

:endproc
echo "juliusrtc is already up"

:end

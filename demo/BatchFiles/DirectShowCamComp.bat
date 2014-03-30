REM ''' Usage of this file
REM %1 usb drive to be used
REM '''

set usbdrive=%1
%usbdrive%
cd demo\BatchFiles
echo Moved to DIR=%cd%

tasklist |findstr "DirectShowCamComp.exe" > nul

if %ERRORLEVEL% == 0  goto endproc

rem start .\CvMonitor.lnk
cd ..\..\misc\OpenRTM-aist\1.1\examples\C++\opencv-rtcs
start .\DirectShowCamComp.exe

cd ..\..\..\..\..\..\demo\BatchFiles

goto end

:endproc
echo "DirectShowCamComp is already up"

:end

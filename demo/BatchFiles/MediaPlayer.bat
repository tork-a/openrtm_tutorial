REM ''' Usage of this file
REM %1 usb drive to be used
REM '''

set usbdrive=%1
%usbdrive%
cd demo\BatchFiles
echo Moved to DIR=%cd%

tasklist |findstr "MediaPlayer.exe" > nul

if %ERRORLEVEL% == 0  goto endproc

cd ..\Apps
start .\MediaPlayer.exe .\mplayer.conf
cd ..\BatchFiles

goto end

:endproc
echo "MediaPlayer is already up"

:end

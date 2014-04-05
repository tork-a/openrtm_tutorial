REM ''' Usage of this file
REM %1 usb drive to be used
REM '''

set usbdrive=%1
%usbdrive%   REM Changing directory to the USB drive.
cd openrtm_tutorial\demo\0_SpeechDemo
echo Moved to DIR=%cd%

call ..\BatchFiles\MediaPlayer.bat
call ..\BatchFiles\PowerPointServer.bat

start ..\OpenHRIAudio\portaudioinput.exe
call ..\BatchFiles\Julius.bat
call ..\BatchFiles\SEAT2.bat



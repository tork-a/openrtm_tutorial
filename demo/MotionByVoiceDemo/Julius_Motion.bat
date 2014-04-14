REM ''' Usage of this file
REM %1 usb drive to be used
REM '''

set usbdrive=%1
%usbdrive%
cd openrtm_tutorial\demo\MotionByVoiceDemo
echo Moved to DIR=%cd%

tasklist |findstr "juliusrtc.exe" > nul

if %ERRORLEVEL% == 0  goto endproc

start ..\OpenHRIVoice\juliusrtc.exe ..\MotionByVoiceDemo\motion.grxml

goto end

:endproc
echo "juliusrtc is already up"

:end

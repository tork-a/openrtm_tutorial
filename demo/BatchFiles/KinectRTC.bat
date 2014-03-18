tasklist |findstr "KinectRTCComp.exe" > nul

if %ERRORLEVEL% == 0  goto endproc

rem start .\KinectRTC.lnk
cd ..\Rtc_KinectRTC
start .\KinectRTCComp.exe
cd ..\BatchFiles

goto end

:endproc
echo "KinectRTC is already up"

:end

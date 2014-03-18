tasklist |findstr "GRobotRTCComp.exe" > nul

if %ERRORLEVEL% == 0  goto endproc

rem start .\CvMonitor.lnk
cd ..\GROBOT_vs10\bin
start .\GRobotRTCComp.exe -f rtc_COM5.conf

cd ..\..\BatchFiles

goto end

:endproc
echo "GRobotRTCComp is already up"

:end

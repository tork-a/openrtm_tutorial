tasklist |findstr "CvMonitorComp.exe" > nul

if %ERRORLEVEL% == 0  goto endproc

rem start .\CvMonitor.lnk
cd ..\Rtc_CvMonitor
start .\CvMonitorComp.exe

cd ..\BatchFiles

goto end

:endproc
echo "CvMonitor is already up"

:end

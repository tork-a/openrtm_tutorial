tasklist |findstr "StateOutComp.exe" > nul

if %ERRORLEVEL% == 0  goto endproc

rem start .\CvMonitor.lnk
cd ..\GROBOT_vs10\bin
start .\StateOutComp.exe

cd ..\..\BatchFiles

goto end

:endproc
echo "StateOutComp.exe is already up"

:end

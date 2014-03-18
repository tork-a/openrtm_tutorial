tasklist |findstr "PowerPointServer.exe" > nul
rem tasklist |findstr "PPTVIEW.EXE" > nul :: 2/2/2014 PPTVIEWER was tried but no luck.

if %ERRORLEVEL% == 0  goto endproc

cd ..\Apps
start .\PowerPointServer.exe .\pps.conf
cd ..\BatchFiles

goto end

:endproc
echo "PowerPointServer is already up"

:end

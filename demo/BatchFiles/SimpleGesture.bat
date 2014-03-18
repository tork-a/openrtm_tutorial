tasklist |findstr "SimpleGestureComp.exe" > nul

if %ERRORLEVEL% == 0  goto endproc

rem start .\SimpleGesture.lnk
cd ..\Rtc_SimpleGesture
start .\SimpleGestureComp.exe
cd ..\BatchFiles

goto end

:endproc
echo "SimpleGesture is already up"

:end

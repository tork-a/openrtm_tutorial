tasklist |findstr "SUIT.exe" > nul

if %ERRORLEVEL% == 0  goto endproc

cd ..\SEATSAT
start .\SUIT.exe ..\kinect_demo2.suitml
cd ..\BatchFiles

goto end

:endproc
echo "suit is already up"

:end

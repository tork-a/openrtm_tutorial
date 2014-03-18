tasklist |findstr "SEAT.exe" > nul

if %ERRORLEVEL% == 0  goto endproc

cd ..\SEATSAT
start .\seateditor.exe ..\kinect_demo2.seatml
cd ..\BatchFiles

goto end

:endproc
echo "seat is already up"

:end

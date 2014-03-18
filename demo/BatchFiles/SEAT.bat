tasklist |findstr "SEAT.exe" > nul

if %ERRORLEVEL% == 0  goto endproc

cd ..\SEATSAT
start .\SEAT.exe ..\kinect_demo.seatml
cd ..\BatchFiles

goto end

:endproc
echo "SEAT is already up"

:end

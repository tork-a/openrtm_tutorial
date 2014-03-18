rem Adding another state to motion_cnoid.seatml

tasklist |findstr "SEAT.exe" > nul

if %ERRORLEVEL% == 0  goto endproc

start ..\SEATSAT\SEAT.exe motion_cnoid2.seatml

goto end

:endproc
echo "SEAT is already up"

:end

tasklist |findstr "SEAT.exe" > nul

if %ERRORLEVEL% == 0  goto endproc

start ..\SEATSAT\SEAT.exe ..\MotionByVoiceDemo\motion_cnoid.seatml

goto end

:endproc
echo "SEAT is already up"

:end

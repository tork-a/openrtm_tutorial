tasklist |findstr "Skelton2JointAngleComp.ex" > nul

if %ERRORLEVEL% == 0  goto endproc

rem start .\Skelton2JointAngle.lnk
cd ..\Rtc_Skelton2JointAngle
start .\Skelton2JointAngleComp.exe
cd ..\BatchFiles

goto end

:endproc
echo "Skelton2JointAngle is already up"

:end

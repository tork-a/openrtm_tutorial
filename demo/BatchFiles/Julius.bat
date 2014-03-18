tasklist |findstr "juliusrtc.exe" > nul

if %ERRORLEVEL% == 0  goto endproc

cd ..\OpenHRIVoice
start .\juliusrtc.exe ..\kinect_demo2.grxml
cd ..\BatchFiles

goto end

:endproc
echo "juliusrtc is already up"

:end

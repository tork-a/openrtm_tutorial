rem Adding another state

tasklist |findstr "juliusrtc.exe" > nul

if %ERRORLEVEL% == 0  goto endproc

start ..\OpenHRIVoice\juliusrtc.exe motion2.grxml

goto end

:endproc
echo "juliusrtc is already up"

:end

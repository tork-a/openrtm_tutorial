tasklist |findstr "MediaPlayer.exe" > nul

if %ERRORLEVEL% == 0  goto endproc

cd ..\Apps
start .\MediaPlayer.exe .\mplayer.conf
cd ..\BatchFiles

goto end

:endproc
echo "MediaPlayer is already up"

:end

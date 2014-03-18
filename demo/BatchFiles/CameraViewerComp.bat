tasklist |findstr "CameraViewerComp.exe" > nul

if %ERRORLEVEL% == 0  goto endproc

cd ..\..\misc/OpenRTM-aist/1.1/examples/C++/opencv-rtcs
start .\CameraViewerComp.exe

cd ..\..\..\..\..\..\demo\BatchFiles

goto end

:endproc
echo "CameraViewerComp is already up"

:end

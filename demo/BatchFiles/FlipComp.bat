tasklist |findstr "FlipComp.exe" > nul

if %ERRORLEVEL% == 0  goto endproc

rem start .\CvMonitor.lnk
cd ..\..\misc\OpenRTM-aist\1.1\examples\C++\opencv-rtcs
start .\FlipComp.exe

cd ..\..\..\..\..\..\demo\BatchFiles

goto end

:endproc
echo "FlipComp is already up"

:end

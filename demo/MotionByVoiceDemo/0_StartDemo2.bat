rem Adding another state

start ..\OpenHRIAudio\portaudioinput.exe
call Julius_Motion2.bat
call SEAT_Motion2.bat
call Choreonoid-GRobot_withMotion.bat

rem For debug
start ..\OpenHRIAudio\portaudiooutput.exe
rem call ..\SampleWordLogger\build\Debug\SampleWordLoggerComp.exe

rem These processes are added by trial to mimic "0 SpeechDemo"
start ..\OpenHRIAudio\portaudioinput.exe
call ..\MotionByVoiceDemo\Julius_Motion.bat
call ..\MotionByVoiceDemo\SEAT_Motion.bat
call ..\MotionByVoiceDemo\Choreonoid-GRobot_withMotion.bat

rem For debug
rem start ..\OpenHRIAudio\portaudiooutput.exe
rem call ..\SampleWordLogger\build\Debug\SampleWordLoggerComp.exe

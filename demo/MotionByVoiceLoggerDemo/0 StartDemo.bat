rem setup MotionByVoiceDemo first.
call ..\MotionByVoiceDemo\"0 StartDemo.bat"

rem setup SampleWordLogger
cd ..\SampleWordLogger\build\Debug
call SampleWordLoggerComp.exe
cd ..\..

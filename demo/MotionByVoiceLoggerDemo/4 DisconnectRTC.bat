for /f "usebackq" %%h in (`hostname`) do set HOSTNAME=%%h

call ..\MotionByVoiceDemo\"4 DisconnectRTC.bat"

call ..\Apps\rtshell\rtdis 127.0.0.1/%HOSTNAME%.host_cxt/SampleWordLogger0.rtc


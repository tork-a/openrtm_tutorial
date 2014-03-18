for /f "usebackq" %%h in (`hostname`) do set HOSTNAME=%%h

call ..\MotionByVoiceDemo\"3 DeactivateRTC.bat"

call ..\Apps\rtshell\rtdeact 127.0.0.1/%HOSTNAME%.host_cxt/SampleWordLogger0.rtc


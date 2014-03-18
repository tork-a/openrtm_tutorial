for /f "usebackq" %%h in (`hostname`) do set HOSTNAME=%%h

call ..\MotionByVoiceDemo\"2 ActivateRTC.bat"

call ..\Apps\rtshell\rtact 127.0.0.1/%HOSTNAME%.host_cxt/SampleWordLogger0.rtc



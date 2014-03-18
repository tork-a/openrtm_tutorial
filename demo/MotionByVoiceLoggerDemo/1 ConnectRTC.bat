
for /f "usebackq" %%h in (`hostname`) do set HOSTNAME=%%h

call "..\MotionByVoiceDemo\1 ConnectRTC.bat"

call ..\Apps\rtshell\rtcon 127.0.0.1/%HOSTNAME%.host_cxt/JuliusRTC0.rtc:result 127.0.0.1/%HOSTNAME%.host_cxt/SampleWordLogger0.rtc:result

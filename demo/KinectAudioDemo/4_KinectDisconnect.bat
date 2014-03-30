
for /f "usebackq" %%h in (`hostname`) do set HOSTNAME=%%h


call ..\Apps\rtshell\rtdis 127.0.0.1/%HOSTNAME%.host_cxt/KinectRTC0.rtc
call ..\Apps\rtshell\rtdis 127.0.0.1/%HOSTNAME%.host_cxt/Skelton2JointAngle0.rtc
rem call rtdis 127.0.0.1/%HOSTNAME%.host_cxt/SEAT0.rtc

call ..\Apps\rtshell\rtdis 127.0.0.1/%HOSTNAME%.host_cxt/SimpleGesture0.rtc
call ..\Apps\rtshell\rtdis 127.0.0.1/%HOSTNAME%.host_cxt/JuliusRTC0.rtc

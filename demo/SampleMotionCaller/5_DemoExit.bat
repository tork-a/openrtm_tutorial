for /f "usebackq" %%h in (`hostname`) do set HOSTNAME=%%h

call ..\Apps\rtshell\rtexit 127.0.0.1/%HOSTNAME%.host_cxt/Cnoid_RobotMotion0.rtc
call ..\Apps\rtshell\rtexit 127.0.0.1/%HOSTNAME%.host_cxt/SampleMotionCaller0.rtc

taskkill /F /IM SampleMotionCaller.exe
taskkill /F /IM CHOREONOID.exe


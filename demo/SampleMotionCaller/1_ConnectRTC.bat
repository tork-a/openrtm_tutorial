
for /f "usebackq" %%h in (`hostname`) do set HOSTNAME=%%h

call ..\Apps\rtshell\rtcon 127.0.0.1/%HOSTNAME%.host_cxt/SampleMotionCaller0.rtc:output 127.0.0.1/%HOSTNAME%.host_cxt/Cnoid_RobotMotion0.rtc:command


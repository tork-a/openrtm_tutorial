for /f "usebackq" %%h in (`hostname`) do set HOSTNAME=%%h


call ..\Apps\rtshell\rtdeact 127.0.0.1/%HOSTNAME%.host_cxt/KinectRTC0.rtc
call ..\Apps\rtshell\rtdeact 127.0.0.1/%HOSTNAME%.host_cxt/CvMonitor0.rtc
call ..\Apps\rtshell\rtdeact 127.0.0.1/%HOSTNAME%.host_cxt/Skelton2JointAngle0.rtc
call ..\Apps\rtshell\rtdeact 127.0.0.1/%HOSTNA..\Apps\rtshell\ME%.host_cxt/SEAT0.rtc
call ..\Apps\rtshell\rtdeact 127.0.0.1/%HOSTNAME%.host_cxt/SimpleGesture0.rtc
call ..\Apps\rtshell\rtdeact 127.0.0.1/%HOSTNAME%.host_cxt/Cnoid_RobotMotion0.rtc

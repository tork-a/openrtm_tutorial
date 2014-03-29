
for /f "usebackq" %%h in (`hostname`) do set HOSTNAME=%%h

call ..\Apps\rtshell\rtcon 127.0.0.1/%HOSTNAME%.host_cxt/KinectRTC0.rtc:rawImage 127.0.0.1/%HOSTNAME%.host_cxt/CvMonitor0.rtc:rawImage
call ..\Apps\rtshell\rtcon 127.0.0.1/%HOSTNAME%.host_cxt/KinectRTC0.rtc:tracking 127.0.0.1/%HOSTNAME%.host_cxt/CvMonitor0.rtc:points
call ..\Apps\rtshell\rtcon 127.0.0.1/%HOSTNAME%.host_cxt/KinectRTC0.rtc:skeleton 127.0.0.1/%HOSTNAME%.host_cxt/Skelton2JointAngle0.rtc:skeleton
call ..\Apps\rtshell\rtcon 127.0.0.1/%HOSTNAME%.host_cxt/KinectRTC0.rtc:skeleton 127.0.0.1/%HOSTNAME%.host_cxt/SimpleGesture0.rtc:skeleton
call ..\Apps\rtshell\rtcon 127.0.0.1/%HOSTNAME%.host_cxt/JuliusRTC0.rtc:data 127.0.0.1/%HOSTNAME%.host_cxt/KinectRTC0.rtc:audio_signal
call ..\Apps\rtshell\rtcon 127.0.0.1/%HOSTNAME%.host_cxt/SEAT0.rtc:gesture 127.0.0.1/%HOSTNAME%.host_cxt/SimpleGesture0.rtc:command
call ..\Apps\rtshell\rtcon 127.0.0.1/%HOSTNAME%.host_cxt/SEAT0.rtc:gesture 127.0.0.1/%HOSTNAME%.host_cxt/SimpleGesture0.rtc:left_hand
call ..\Apps\rtshell\rtcon 127.0.0.1/%HOSTNAME%.host_cxt/SEAT0.rtc:gesture 127.0.0.1/%HOSTNAME%.host_cxt/SimpleGesture0.rtc:right_hand
call ..\Apps\rtshell\rtcon 127.0.0.1/%HOSTNAME%.host_cxt/SEAT0.rtc:command 127.0.0.1/%HOSTNAME%.host_cxt/Skelton2JointAngle0.rtc:command
call ..\Apps\rtshell\rtcon 127.0.0.1/%HOSTNAME%.host_cxt/JuliusRTC0.rtc:result 127.0.0.1/%HOSTNAME%.host_cxt/SEAT0.rtc:gesture


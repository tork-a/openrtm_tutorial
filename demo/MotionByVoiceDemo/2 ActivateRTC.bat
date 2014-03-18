for /f "usebackq" %%h in (`hostname`) do set HOSTNAME=%%h

call ..\Apps\rtshell\rtact 127.0.0.1/%HOSTNAME%.host_cxt/SEAT0.rtc
call ..\Apps\rtshell\rtact 127.0.0.1/%HOSTNAME%.host_cxt/Cnoid_RobotMotion0.rtc

rem These processes are added by trial to mimic "0 SpeechDemo"
call ..\Apps\rtshell\rtact 127.0.0.1/%HOSTNAME%.host_cxt/JuliusRTC0.rtc
call ..\Apps\rtshell\rtact 127.0.0.1/%HOSTNAME%.host_cxt/PortAudioInput0.rtc

rem call ..\Apps\rtshell\rtact 127.0.0.1/%HOSTNAME%.host_cxt/PortAudioOutput0.rtc

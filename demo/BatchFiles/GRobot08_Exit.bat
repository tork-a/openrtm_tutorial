for /f "usebackq" %%h in (`hostname`) do set HOSTNAME=%%h

call rtexit 127.0.0.1/%HOSTNAME%.host_cxt/GRobot08.rtc




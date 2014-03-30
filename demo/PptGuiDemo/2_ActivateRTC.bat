for /f "usebackq" %%h in (`hostname`) do set HOSTNAME=%%h


call ..\Apps\rtshell\rtact 127.0.0.1/%HOSTNAME%.host_cxt/SUIT0.rtc
call ..\Apps\rtshell\rtact 127.0.0.1/%HOSTNAME%.host_cxt/SEAT0.rtc



for /f "usebackq" %%h in (`hostname`) do set HOSTNAME=%%h

call ..\Apps\rtshell\rtcon 127.0.0.1/%HOSTNAME%.host_cxt/SEAT0.rtc:gesture 127.0.0.1/%HOSTNAME%.host_cxt/SUIT0.rtc:command

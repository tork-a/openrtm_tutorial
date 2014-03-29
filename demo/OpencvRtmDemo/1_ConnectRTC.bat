for /f "usebackq" %%h in (`hostname`) do set HOSTNAME=%%h

rem call ..\Apps\rtshell\rtcon 127.0.0.1/%HOSTNAME%.host_cxt/DirectShowCam0.rtc:m_FIN_OUTPORT 127.0.0.1/%HOSTNAME%.host_cxt/Flip0.rtc:original_image
rem call ..\Apps\rtshell\rtcon 127.0.0.1/%HOSTNAME%.host_cxt/Flip0.rtc:fliped_image.in 127.0.0.1/%HOSTNAME%.host_cxt/SEAT0.rtc:fliped_image

call ..\Apps\rtshell\rtcon 127.0.0.1/DirectShowCam0.rtc:m_FIN_OUTPORT 127.0.0.1/Flip0.rtc:original_image
call ..\Apps\rtshell\rtcon 127.0.0.1/Flip0.rtc:fliped_image.in 127.0.0.1/SEAT0.rtc:fliped_image
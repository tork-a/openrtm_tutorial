set usbdrive=%1
%usbdrive%   REM Changing directory to the USB drive.
cd openrtm_tutorial\demo\0_SpeechDemo
echo Moved to DIR=%cd%

for /f "usebackq" %%h in (`hostname`) do set HOSTNAME=%%h

rem call ..\Apps\rtshell\rtcon 127.0.0.1/%HOSTNAME%.host_cxt/DirectShowCam0.rtc:m_FIN_OUTPORT 127.0.0.1/%HOSTNAME%.host_cxt/Flip0.rtc:original_image
rem call ..\Apps\rtshell\rtcon 127.0.0.1/%HOSTNAME%.host_cxt/Flip0.rtc:fliped_image.in 127.0.0.1/%HOSTNAME%.host_cxt/SEAT0.rtc:fliped_image

rem call ..\Apps\rtshell\rtcon 127.0.0.1/DirectShowCam0.rtc:m_FIN_OUTPORT 127.0.0.1/Flip0.rtc:original_image
rem call ..\Apps\rtshell\rtcon 127.0.0.1/Flip0.rtc:fliped_image.in 127.0.0.1/SEAT0.rtc:fliped_image

call ..\Apps\rtshell\rtcon 127.0.0.1/%HOSTNAME%.host_cxt/VS_ASR_RTC0.rtc:ManipulatorCommonInterface_Common 127.0.0.1/%HOSTNAME%.host_cxt/ScaraRobotControlRTC0.rtc:ManipulatorCommonInterface_Common
call ..\Apps\rtshell\rtcon 127.0.0.1/%HOSTNAME%.host_cxt/VS_ASR_RTC0.rtc:ManipulatorCommonInterface_Middle 127.0.0.1/%HOSTNAME%.host_cxt/ScaraRobotControlRTC0.rtc:ManipulatorCommonInterface_Middle

rem timeout /T 2 /NOBREAK

call ..\Apps\rtshell\rtact 127.0.0.1/%HOSTNAME%.host_cxt/VS_ASR_RTC0.rtc
call ..\Apps\rtshell\rtact 127.0.0.1/%HOSTNAME%.host_cxt/ScaraRobotControlRTC0.rtc
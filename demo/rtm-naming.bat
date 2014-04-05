@echo off

REM ''' Usage of this file
REM %1 usb drive to be used
REM %2 port num for omninames. Should be 2809 unless there's a reason.
REM '''

set usbdrive=%1
%usbdrive%   REM Changing directory to the USB drive.
cd openrtm_tutorial\demo  
echo Moved to DIR=%cd%

rem set cosnames="omninames"
set cosnames="omniNames"
set orb="omniORB"
set port=%2
rem set OMNIORB_USEHOSTNAME=localhost
set OMNI_ROOT=..\misc\OpenRTM-aist\1.1\omniORB\4.1.5
set PATH=%PATH%;%OMNI_ROOT%\bin\x86_win32

if NOT DEFINED port set port=2809

for /f %%x in ('hostname') do @set hosts=%%x

if %orb%=="omniORB" goto omni
if %orb%=="tao" goto tao
goto other

:omni
rem if exist %cosnames%  echo "ok, cosnames exists."
echo hostname = %hosts% TEMPFILE = %TEMP%\omninames-%hosts%.log
if EXIST %TEMP%\omninames-%hosts%.log del /F %TEMP%\omninames-%hosts%.log
if EXIST %TEMP%\omninames-%hosts%.bak del /f %TEMP%\omninames-%hosts%.bak
rem echo Debug) PATH = %PATH%

echo CURRENT DIR=%cd%
echo Starting omniORB omniNames: %hosts%:%port%
cmd.exe /K ..\misc\OpenRTM-aist\1.1\tools\omniNames.exe -start %port% -logdir %TEMP%\


goto:EOF

:tao
echo Starting TAO Naming_Service: %hosts%:%port%
%cosnames% -ORBEndPoint iiop://%hostname%:%port%
goto:EOF

:other
echo "other proc"
goto:EOF

:err
echo Usage: rtm-naming port_number
goto:EOF


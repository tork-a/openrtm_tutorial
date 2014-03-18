@echo off

rem set cosnames="omninames"
set cosnames="omniNames"
set orb="omniORB"
set port=%1
rem set OMNIORB_USEHOSTNAME=localhost
rem set OMNI_ROOT=c:\Users\n130s\data\ExistingRsc_RtmUSB_201312\OpenRTM-aist\1.1\omniORB\4.1.5
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
echo Debug) PATH = %PATH%
echo Starting omniORB omniNames: %hosts%:%port%
%cosnames% -start %port% -logdir %TEMP%\

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


@echo off

echo #############################################
echo # Self Update Program of openrtm_tutorial   #
echo #                                           #
echo # TORK (opensource-robotics.tokyo.jp)       #
echo #############################################
echo 
echo Now git pull from github.com

PATH=%PATH%:PortableGit-1.9.0preview\bin
PortableGit-1.9.0preview\bin\git pull
timeout -1

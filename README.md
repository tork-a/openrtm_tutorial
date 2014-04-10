**OpenRTM Tutorial**

Project Introduction
====================
Compilation of existing tutorial contents, with a few new tutorials being added. 
Contents are originally intended to be distributed packaged in USB drive.

System Environment Requirement
==============================
 * Internet access
 * Windows 7 / 8
 * Internet Explorer (tested 11)

To Start Tutorial
=================
Tutorial documents are placed in `%TUTORIAL_HOME%/doc`. Open `%TUTORIAL_HOME%/index.htm`.

Online Update
=============
While almost all the tutorial material included herein is hosted and maintained online on a github repository [1_], you can update your USB drive easily by a normal "git-way". To do so:

 * Double-click `%TUTORIAL_HOME%/SELFUPDATE.bat`; this runs `git pull` command and synchronizes data in the drive with the repository.
 * It's possible that conflict occurs if you've added change to your USB drive. In this case, conflict may occur and currently no pre/post caution measure is implemented. Please take care of it in "git-way" by yourself.

If You Find Issues
==================
Please report at the issue tracker [2_].

For Maintainers
=================
To update tutorial html files, it is strongly recommended not to modify `.htm` files directly, and instead you add changes to `.rst` files.
for the versatility to realize platform-agnosticity, 

Funding, Special Thanks
=======================
Project funded by New Energy and Industrial Technology Development Organization (NEDO). Special thanks to Dr. Hara (AIST), Dr. Yuki Suga (Sugar Sweet Robotics).

.. _1: https://github.com/tork-a/openrtm_tutorial
.. _2: https://github.com/tork-a/openrtm_tutorial/issues?direction=desc&sort=updated&state=open
====================
OpenRTM Tutorial
====================

Project Introduction
====================
Compilation of existing tutorial contents, with a few new tutorials being added. Contents are originally intended to be distributed packaged in USB drive.

System Environment Requirement
==============================
 * Internet access
 * Windows 7 / 8
 * A microphone

To Start Tutorial
=================
Open `%TUTORIAL_HOME%/index.htm` (Tutorial documents are placed in `%TUTORIAL_HOME%/doc`). 

Online Update
=============
While almost all the tutorial material included herein is hosted and maintained online on a github repository [1_], you can update your USB drive easily by a normal "git-way". To do so:

 * Double-click `%TUTORIAL_HOME%/SELFUPDATE.bat`; this runs `git pull` command and synchronizes data in the drive with the repository.
 * It's possible that conflict occurs if you've added change to your USB drive. In this case, conflict may occur and currently no pre/post caution measure is implemented. Please take care of it in "git-way" by yourself.

If You Find Issues
==================
Please report at the issue tracker [2_].

For Tutorial Maintainers
=========================

Maintainer Prerequisite
-----------------------
Ubuntu or other platform where the following software is available is assumed:

 * `git`
 * `sphinx-intl` (via `pip install sphinx-intl`)

git repository branch
---------------------
To keep the released version and development version, let's use following two branches:

- `devel`: Default. All developers and maintainers use this branch for the work.
- `master`: Released version. `SELFUPDATE.bat` only accesses this branch.

Discussion about this is found at [5_].

To Create USB
-------------
You can freely create a USB thumbdrive using data maintained on the github repository [1_].

On a computer where `git` is available, do::

    $ cd %YOUR_USBDRIVE%
    $ git clone https://github.com/tork-a/openrtm_tutorial.git

That's it.

NOTE: Above said, the folder structure in USB must maintain `openrtm_tutorial` at the top folder like this:

    %USBDRIVE%:\openrtm_tutorial
    F:\openrtm_tutorial          (e.g. If the USB is recognized as F drive)

Notice for updating tutorial html files
-----------------------------------------
Please do NOT modify `.html` (that you find under doc/_build, doc/locale) files directly. Instead, add changes to `doc/*.rst` files. 

Those `.rst` files are currently written in Japanese only, but English is available too for the generated `.html`. That said, currently there's no way to update the document BY typing in English.

To update tutorial html
-------------------------
Both `ja` and `en` .html files are generated from `.rst` files by using `sphinx-intl` [3_]. Several steps need to be run to generate .html files for both per language. Every time a change is to be applied to the source `.rst`, a maintainer run these steps. 

*Note*: These processes may be insufficient in some situation. Your feedback is appreciated at [2_].

0. It's strongly recommended that you create a git `branch`, as always when you work on `git`.
1. Add change to `.rst`
2. Run these commands.::


    $ cd %OPENRTM_TUTORIAL%/doc            (where conf.py exists)
    $ make gettext
    $ sphinx-intl update -p _build/locale -l en -l ja


3. Run also these.::

    $ cd %OPENRTM_TUTORIAL%/doc            (in case you're away)
    $ sphinx-intl build
    $ make -e SPHINXOPTS="-D language='ja'" html   (not ideal, but "make"-ing `en` directly just skips for some reasons)
    $ cp -R _build/html/* locale/ja   (Copy translation .html files to manually created folder `locale`)
    $ make -e SPHINXOPTS="-D language='en'" html
    $ cp -R _build/html/* locale/en   (Copy translation .html files to manually created folder `locale`)


To update translation
-------------------------
To update ONLY English translation, modify the corresponding `%OPENRTM_TUTORIAL%/doc/locale/en/LC_MESSAGES/.po` files (intermediate files that `sphinx-intl` uses to allow translation). Then run `3` in previous section.


Funding, Special Thanks
=======================
Project funded by New Energy and Industrial Technology Development Organization (NEDO). Special thanks to Dr. Hara (AIST), Dr. Yuki Suga (Sugar Sweet Robotics).

.. _1: https://github.com/tork-a/openrtm_tutorial
.. _2: https://github.com/tork-a/openrtm_tutorial/issues?direction=desc&sort=updated&state=open
.. _3: http://sphinx-doc.org/latest/intl.html
.. _4: https://github.com/tork-a/openrtm_tutorial/issues/20
.. _5: https://github.com/tork-a/openrtm_tutorial/issues/29

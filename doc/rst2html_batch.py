#!/usr/bin/python

import os
from subprocess import call

_EXT_HTM = '.htm'
_EXT_RST = '.rst'

rst_files = [f for f in os.listdir(os.path.curdir) if f.endswith(_EXT_RST)]
for f in rst_files:
    fname_root, fextension = os.path.splitext(f)
    call(['rst2html', fname_root + _EXT_RST, fname_root + _EXT_HTM])
    print(fname_root + _EXT_HTM + 'is generated')


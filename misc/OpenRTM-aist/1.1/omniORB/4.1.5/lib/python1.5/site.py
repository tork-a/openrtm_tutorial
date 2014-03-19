import sys

# For omnipython, manually set sys.prefix if it isn't already set
if sys.prefix == "":
    import os
    currprefix = os.path.dirname(sys.executable)
    lastprefix = ""

    while currprefix != lastprefix:
        if os.path.isdir(os.path.join(currprefix, "lib", "python1.5")):
            sys.exec_prefix = sys.prefix = currprefix
            break
        lastprefix = currprefix
        currprefix = os.path.dirname(currprefix)

    del currprefix, lastprefix

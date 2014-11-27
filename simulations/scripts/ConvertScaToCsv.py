#!/usr/bin/python

import sys
import re
import subprocess
import os

files = []
if len(sys.argv) < 2:
    for l in os.listdir("."):
        m = re.search("(\w+)-(\d+).sca", l)
        if m:
            files.append(l)

else:
    files = sys.argv[1:]

for l in files:
    m = re.search("(\w+)-(\d+).sca", l)
    if m:
        c = m.group(1)
        r = int(m.group(2))
        subprocess.call("scavetool scalar -O {config}-{run}.csv {config}-{run}.sca".format(config=c, run=r), shell=True)

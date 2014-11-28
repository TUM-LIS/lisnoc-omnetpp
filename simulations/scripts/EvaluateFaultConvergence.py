#!/usr/bin/python

import sys, re, os
from matplotlib import pyplot

files = []

if len(sys.argv) < 2:
    for l in os.listdir("."):
        if re.match("(\w+)-(\d+).stdout", l):
            files.append(l)
else:
    files = sys.argv[1:]

files.sort()

conv = []

for i in files:
    f = open(i)

    results = [0]

    for l in f:
        m = re.search("\d+(\.\d+)? CONVERGENCE: (\d(\.\d+)?(e-\d+)?)", l)
        if m:
            results.append(m.group(2))

    conv.append(results)

print conv

for c in conv:
    pyplot.plot(c)
    pyplot.show()




#!/usr/bin/python

import sys, re, os
from matplotlib import pyplot

def evaluate(file):
    f = open(file)

    mean = [0]
    stddev = [0, 0]

    for l in f:
        m = re.search("\d+(\.\d+)? .* CONVERGENCE: (\d(\.\d+)?(e-\d+)?) STDDEV: (\d(\.\d+)?(e-\d+)?)", l)
        if m:
            if m.group(2) != "-nan":
                mean.append(m.group(2))
            if m.group(5) != "-nan":
                stddev.append(m.group(5))

    return { "mean":  mean, "stddev": stddev }

if __name__ == "__main__":
    files = []

    if len(sys.argv) < 2:
        for l in os.listdir("."):
            if re.match("(\w+)-(\d+).stdout", l):
                files.append(l)
    else:
        files = sys.argv[1:]

    files.sort()

    for i in files:
        print i, extract(i)




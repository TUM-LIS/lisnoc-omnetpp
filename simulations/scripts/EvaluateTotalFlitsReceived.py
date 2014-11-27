#!/usr/bin/python

import sys, re, os

files = []

if len(sys.argv) < 2:
    for l in os.listdir("."):
        if re.match("(\w+)-(\d+).csv", l):
            files.append(l)
else:
    files = sys.argv[1:]

files.sort()

for i in files:
    f = open(i)

    total = 0

    for l in f:
        m = re.search("ni\[(\d+)\].statisticsUnit,flit_network_latency:count,(\d+)", l)
        if m:
            total = total + int(m.group(2))

    print total


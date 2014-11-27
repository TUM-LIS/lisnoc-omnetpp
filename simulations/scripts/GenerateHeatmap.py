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

    heatcollect = {}
    heat = {}

    for l in f:
        m = re.search("router\[(\d+)\].statisticsUnit,inBuffer_(\d+)_vc_(\d+)_latency:mean,(\d+(\.\d+)?)", l)
        if m:
            idx = int(m.group(1))
            mean = float(m.group(4))
            if mean > 0:
                if idx not in heatcollect:
                    heatcollect[idx] = []
                heatcollect[idx].append(mean)
        m = re.search("router\[(\d+)\].statisticsUnit,outBuffer_(\d+)_vc_(\d+)_latency:mean,(\d+(\.\d+)?)", l)
        if m:
            idx = int(m.group(1))
            mean = float(m.group(4))
            if mean > 0:
                if idx not in heatcollect:
                    heatcollect[idx] = []
                heatcollect[idx].append(mean)

    for idx in heatcollect.keys():
        heat[idx] = sum(heatcollect[idx])/len(heatcollect[idx])

    print heat

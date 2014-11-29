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

    acc = []
    net = []
    total = []

    for l in f:
        m = re.search("ni\[(\d+)\].statisticsUnit,flit_network_latency:mean,(\d+(\.\d+)?)", l)
        if m:
            net.append(float(m.group(2)))
        m = re.search("ni\[(\d+)\].statisticsUnit,flit_network_access_latency:mean,(\d+(\.\d+)?)", l)
        if m:
            acc.append(float(m.group(2)))
        m = re.search("ni\[(\d+)\].statisticsUnit,flit_total_latency:mean,(\d+(\.\d+)?)", l)
        if m:
            total.append(float(m.group(2)))

    acc = [a for a in acc if a > 0]
    net = [a for a in net if a > 0]
    total = [a for a in total if a > 0]

    print i, sum(acc)/len(acc),sum(net)/len(net),sum(total)/len(total)


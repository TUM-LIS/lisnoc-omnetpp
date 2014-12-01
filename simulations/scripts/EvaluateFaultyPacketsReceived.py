#!/usr/bin/python

import sys, re, os

def evaluate(file):
    total = 0
    faulty = 0

    f = open(file)

    for l in f:
        m = re.search("ni\[(\d+)\].statisticsUnit,packet_arrived_faulty:count,(\d+(.(\d+)e\+(\d+))?)", l)
        if m:
            if m.group(3):
                raise(BaseException("ERROR: we need precise numbers"))
            else:
                total = total + int(m.group(2))

        m = re.search("ni\[(\d+)\].statisticsUnit,packet_arrived_faulty:sum,(\d+(.(\d+)e\+(\d+))?)", l)
        if m:
            if m.group(3):
                raise(BaseException("ERROR: we need precise numbers"))
            else:
                faulty = faulty + int(m.group(2))
    
    return { 'total': total, 'faulty': faulty }

if __name__ == "__main__":
    files = []

    if len(sys.argv) < 2:
        for l in os.listdir("."):
            if re.match("(\w+)-(\d+).csv", l):
                files.append(l)
    else:
        files = sys.argv[1:]

    files.sort()

    for i in files:
        print evaluate(i)



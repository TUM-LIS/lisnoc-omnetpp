#!/usr/bin/python

from multiprocessing import Pool
import subprocess
import sys
from datetime import datetime
import re
import os

def execute(job):
    stdout = "{0}/{1}-{2}.stdout".format(job["result-dir"],job["config"],job["run"])
    stderr = "{0}/{1}-{2}.stderr".format(job["result-dir"],job["config"],job["run"])
    execstring = "../src/lisnoc -u Cmdenv -n .:../src -c {config} -r {run} --result-dir={results}".format(config=job["config"],run=job["run"],results=job["result-dir"])
    try:
        subprocess.call(execstring, stdout=open(stdout,"w"), stderr=open(stderr,"w"), shell=True)
    except KeyboardInterrupt:
        print "(Ctrl-C)"

if __name__=="__main__":
    if len(sys.argv) < 2:
        print "Usage: RunParallel.py <config>"
        exit(1)
        
    config = sys.argv[1]
    out = subprocess.check_output("../src/lisnoc -n .:../src -x {config}".format(config=config), shell=True)

    m = re.search("Number of runs: (\d+)", out)
    if not m:
        raise "Cannot extract number of runs"

    runs = int(m.group(1))

    jobs = []
    datestr = datetime.now().strftime("%Y%m%d-%H%M%S")
    resultdir = "results-{datestr}".format(datestr=datestr)
    os.mkdir(resultdir)

    for r in range(runs):
        job = {}
        job["config"] = config
        job["result-dir"] = resultdir
        job["run"] = r
        jobs.append(job)

    pool = Pool()
    try:
        pool.map(execute, jobs)
        pool.join()
    except KeyboardInterrupt:
        print "(Ctrl-C)"
        pool.terminate()

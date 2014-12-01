#!/usr/bin/python

import sys, re, os

class Run:
    def __init__(self):
        self.id = None
        self.parameters = {}
        self.config = None

    def getParameter(self, name):
        return self.parameters[name]

    def getScaFilename(self):
        return "{0}-{1}.sca".format(self.config, self.id)

    def getCsvFilename(self):
        return "{0}-{1}.csv".format(self.config, self.id)

class Config:
    def __init__(self):
        self.parameters = []
        self.name = None
        self.runs = None

    def addRuns(self, runs = None):
        if runs is None:
            files = []
            for l in os.listdir("."):
                if re.match("(\w+)-(\d+).sca", l):
                    files.append(l)
            runs = extract(files)

        self.runs = runs
        
        if self.name is None:
            self.name = self.runs[0].config
            self.parameters = self.runs[0].parameters.keys()

    def getRuns(self, fixed):
        free = [p for p in self.parameters if p not in fixed.keys()]
        runs = {}
        for r in self.runs:
            matches = True
            for f in fixed.keys():
                if r.parameters[f] != fixed[f]:
                    matches = False
            if matches:
                freeval = r.parameters[free[0]]
                runs[freeval] = r
        return runs

def extract(files):
    runs = []
    for file in files:
        m = re.match("(\w+)-(\d+).sca", file)
        if not m:
            raise BaseException("Cannot match file")

        f = open(file)

        run = Run()
        run.config = m.group(1)
        run.id = m.group(2)

        for l in f:
            m = re.search("attr iterationvars (\"(.*)\"|\$.*)", l)
            if m:
                iterations = [m.group(1)]
                if m.group(2):
                    iterations = m.group(2).split(',')
                    iterations = [i.strip() for i in iterations]

                p = {}
                for it in iterations:
                    m = re.search("\$(.*)=(.*)", it)
                    if m:
                        p[m.group(1)] = m.group(2)
                run.parameters = p
        runs.append(run)

    return runs

if __name__ == "__main__":
    files = []

    if len(sys.argv) < 2:
        for l in os.listdir("."):
            if re.match("(\w+)-(\d+).sca", l):
                files.append(l)
        files.sort()
    else:
        files = sys.argv[1:]

    runs = extract(files)
    config = Config()
    config.addRuns(runs)
    print "Configuration name:", config.name
    print "Parameters:", ", ".join(config.parameters)
    for r in runs:
        print "Run", r.id, "parameters", r.parameters, "scalar file:", r.getScaFilename()


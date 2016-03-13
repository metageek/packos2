#!/usr/bin/env python

import tempfile, os, sys, subprocess

def which(f):
    def canexec(fpath):
        return os.path.isfile(fpath) and os.access(fpath, os.X_OK)

    if not f:
        return None
    if f[0] in ['/', '.']:
        if canexec(f):
            return f
        else:
            return None
        
    for d in os.getenv('PATH').split(':'):
        fpath = os.path.join(d, f)
        if canexec(fpath):
            return fpath
    return None

def whichdir(f):
    fpath = which(f)
    if not fpath:
        return None
    return os.path.dirname(fpath)

class Assertion(object):
    def __init__(self):
        self.file = None
        self.line = None
        self.matched = None
        self.expr = None
        self.expected = None
        self.actual = None

    def processLine(self, line):
        if not line:
            return

        if line[0] == 'A':
            (_, filename, lineno, success) = line.split(':')
            self.file = filename
            self.line = int(lineno)
            self.matched = (success == 'Y')
            return

        if line[0] == 'e':
            self.expr = line.split(':')[1]
            return

        if line[0] == 'x':
            self.expected = line.split(':')[1]
            return

        if line[0] == 'a':
            self.actual = line.split(':')[1]
            return

    def isComplete(self):
        return (self.file is not None
                and self.expr is not None
                and self.expected is not None
                and self.actual is not None)

    def show(self):
        print '%s in %s, line %d:' % (('PASS' if self.matched else 'FAIL'),
                                      self.file,
                                      self.line)
        print '  Expression: %s' % self.expr
        print '  Expected:   %s' % self.expected
        print '  Actual:     %s' % self.actual

def parseLogfile(f):
    cur = Assertion()

    for line in open(f, 'r'):
        line = line.strip()
        if not line:
            continue
        if line[0] == 'L':
            print line.split(':')[1]
            continue
        cur.processLine(line)
        if cur.isComplete():
            cur.show()
            cur = Assertion()

(logfd, logfile) = tempfile.mkstemp(suffix='.log', prefix='qunit')
os.close(logfd)

scriptdir = whichdir(sys.argv[0])

bootable = sys.argv[1]

subprocess.call(["%s/run" % scriptdir,
                 bootable,
                 logfile])

parseLogfile(logfile)

os.unlink(logfile)

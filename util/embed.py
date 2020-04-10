#!/usr/bin/python

import sys

if len(sys.argv) <> 3:
    print "USAGE: embed varname filename"
    sys.exit(0)
    
length = 0
with open(sys.argv[2], "r") as fp:
    print "const unsigned char %s_data[] = {" % (sys.argv[1], )
    while True:
        x = fp.read(256)
        if len(x) == 0:
            break
        print "\t%s," % (','.join('%d' % (ord(c), ) for c in x))
        length += len(x)
    print "0\n"
    length += 1
    print "};"
    print "const unsigned %s_size = %d;" % (sys.argv[1], length)

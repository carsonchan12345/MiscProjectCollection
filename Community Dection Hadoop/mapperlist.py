#!/usr/bin/env python3.4
"""mapper.py"""

import sys


for line in sys.stdin:

    line = line.strip()

    a,b = line.split()

    print ('%s\t%s' % (a,b))
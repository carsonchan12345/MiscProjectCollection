#!/usr/bin/env python3.4
"""reducer.py"""

from operator import itemgetter
import sys

current_key = None
current_count = 0
key = None
value = None


for line in sys.stdin:

    line = line.strip()


    key , value = line.split("\t")


    if current_key == key:
        current_count+=1
    else:
        if current_key:

            print ('%s\t%s' % (current_key, current_count))
        current_count = 1
        current_key = key

if current_key == key:
    print ('%s\t%s' % (current_key, current_count))
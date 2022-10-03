#!/usr/bin/env python3.4
"""reducer.py"""

from operator import itemgetter
import sys

current_key = None
current_list = []
key = None
value = None

for line in sys.stdin:

    line = line.strip()


    key , value = line.split("\t")

  
    if current_key == key:
        current_list.append(value) 
    else:
        if current_key:
            # write result to STDOUT
            print ('%s\t%s' % (current_key, current_list))
        current_list =[]
        current_list.append(value)
        current_key = key

if current_key == key:
    print ('%s\t%s' % (current_key, current_list))
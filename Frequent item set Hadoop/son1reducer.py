#!/usr/bin/env python3.4
import sys

#a list will be generated as result

current_key=None
current_count=0
for line in sys.stdin:

    line=line.strip(" \n")
    line=line.split("\t")
    if (current_key==line[0]):
        current_count+=int(line[1])
    else:
        if current_key:
            print(current_key)
        current_key=line[0]
        current_count=int(line[1])

    

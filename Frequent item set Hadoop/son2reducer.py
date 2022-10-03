#!/usr/bin/env python3.4
import sys

current_key=None
current_count=0
for line in sys.stdin:

    line=line.strip(" \n")
    line=line.split("\t")
    if (current_key==line[0]):
        current_count+=int(line[1])
    else:
        if current_key:
            if current_count>24923:
                print("%s\t%s"%(current_key,int(current_count)))
        current_key=line[0]
        current_count=int(line[1])

    

#!/usr/bin/env python3.4
import random
import sys

for line in sys.stdin:
    line=line.strip(" \n\r")
    randbasket=str(random.randint(1,5))
    print("%s\t%s" %(randbasket,line))

#!/usr/bin/env python3.4
import random
import sys

for line in sys.stdin:
    line=line.strip(" \n\r")
    line=line.split("\t")
    print(line[1])

#!/usr/bin/env python3.4
"""mapper.py"""

import sys
import ast




for line in sys.stdin:

    line = line.strip(" ")

    a,b = line.split("\t")
  
    b=ast.literal_eval(b) 
    

  
    for x in range(0, len(b)):
        for y in range(0, len(b)):
            if(b[x]!=b[y]):
                print("%s\t%s" % (b[x],b[y]))

    
        
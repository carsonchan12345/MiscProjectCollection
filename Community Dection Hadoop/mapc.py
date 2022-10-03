#!/usr/bin/env python3.7
"""mapper.py"""

import sys
import ast

f=open("twsim2","r")

setlist={}

for line in f:
  line = line.strip()


  key , value = line.split("\t",1)
  value= value.split("\t")
  setlist[key]=value

for line in sys.stdin:

    line = line.strip(" ")

    a,b = line.split("\t")
  
    b=ast.literal_eval(b) 
    


    for x in range(0, len(b)):
        for y in range(0, len(b)):
            if(b[x]!=b[y] and b[y] in setlist[b[x]]):
                print("%s\t%s\t%s" % (b[x],b[y],a))

    
        
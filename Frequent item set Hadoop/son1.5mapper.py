#!/usr/bin/env python3.4
import sys

f=open("pass1dict","r")
pass2dict=dict()

pass1dict=set()
for line in f:
    
    line=line.strip(" \n")
    line=line.split("\t")
    pass1dict.add(line[0])
f.close()

def secondpass():

    basketcount=0
    for line in sys.stdin:
        basketcount+=1
        
        line=line.strip(" \n")
        line=line.split()
        lenline=len(line)
        for x in range(0,lenline):
            for y in range(x,lenline):
                if line[x]==line[y] or line[x] not in pass1dict or line[y] not in pass1dict:
                    continue

                tmp=[line[x],line[y]]
                tmp.sort()
                tmpword=tmp[0]+","+tmp[1]
                if tmpword in pass2dict:
                    pass2dict[tmpword]+=1
                else:
                    pass2dict[tmpword]=1


def mapout():
    for k in pass2dict:
        print("%s\t%s"%(k,str(pass2dict[k])))

secondpass()
mapout()
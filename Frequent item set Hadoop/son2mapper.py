#!/usr/bin/env python3.4
import sys

ps2=set()
f=open("pass2dict","r")
for line in f:
    line=line.strip(" \n\t")
    line=line.split(" ")
    ps2.add(line[0])
f.close()

pass2dict=dict()


def son2map():
    
    for line in sys.stdin:

        line=line.strip(" \n")

        line=line.split()
        lenline=len(line)
        for x in range(0,lenline):
           for y in range(x,lenline):
                tmp=[line[x],line[y]]
                tmp.sort()
                tmpword=tmp[0]+","+tmp[1]
                
                if line[x]==line[y] or tmpword not in ps2:
                    
                    continue
                
                if tmpword in pass2dict:
                    pass2dict[tmpword]+=1
                else:
                    pass2dict[tmpword]=1
def mapout():
    for k in pass2dict:
        print("%s\t%s"%(k,str(pass2dict[k])))

son2map()
mapout()


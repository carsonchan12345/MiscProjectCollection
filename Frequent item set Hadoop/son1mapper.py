#!/usr/bin/env python3.4
import sys



pass1dict=dict()
pass2dict=dict()
subsetbusket=list()

def readinmem():
    for line in sys.stdin:
        subsetbusket.append(line)


def firstpass():
    basketcount=0
    for line in subsetbusket:
        line=line.strip(" \n\r")
        line=line.split()
        for words in line:
            if words in pass1dict:
                pass1dict[words]+=1
            else:
                pass1dict[words]=1
        basketcount+=1
    threshold=0.005*basketcount

    for k in list(pass1dict.keys()):
        if pass1dict[k]<threshold:
            del pass1dict[k]
    


def secondpass():

    basketcount=0
    for line in subsetbusket:
        basketcount+=1
        
        line=line.strip(" \n\r")
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
    threshold=0.005*basketcount
    for k in list(pass2dict.keys()):
        if pass2dict[k]<threshold:
             del pass2dict[k]


def mapout():
    for k in pass2dict:
        print("%s\t%s"%(k,str(pass2dict[k])))

readinmem()
firstpass()
secondpass()
mapout()


    
#!/usr/bin/env python3.4

import sys



subsetbusket=list()
hashbucket=dict()
pass1dict=dict()
pass2dict=dict()

def hashfunction(word1,word2):
    tmp=hash(word1+word2)%1000000
    return tmp

def readinmem():
    for line in sys.stdin:
        subsetbusket.append(line)


def firstpass():
    basketcount=0
    for line in subsetbusket:
        basketcount+=1

        line=line.strip(" \n")
        line=line.split()
        lenline=len(line)
        for x in range(0,lenline):

            if line[x] in pass1dict:
                pass1dict[line[x]]+=1
            else:
                pass1dict[line[x]]=1

            for y in range(x,lenline):
                if line[x]==line[y]:
                    continue

                tmp=[line[x],line[y]]
                tmp.sort()
                tmpword=hashfunction(tmp[0],tmp[1])
                if tmpword in hashbucket:
                    hashbucket[tmpword]+=1
                else:
                    hashbucket[tmpword]=1
                
    threshold=0.005*basketcount

    for k in list(pass1dict.keys()):
        if pass1dict[k]<threshold:
            del pass1dict[k]
        else:
            pass1dict[k]=1

        
    for k in list(hashbucket.keys()):
        if hashbucket[k]<threshold:
            del hashbucket[k]
        else:
            hashbucket[k]=1

    

def secondpass():

    basketcount=0
    for line in subsetbusket:
        basketcount+=1
        
        line=line.strip(" \n")
        line=line.split()
        lenline=len(line)
        for x in range(0,lenline):
            for y in range(x,lenline):

                if line[x]==line[y]:
                    continue
                tmp=[line[x],line[y]]
                tmp.sort()
                tmpword=hashfunction(tmp[0],tmp[1])
                #generate the canidate fullfilling the hash and the freq count
                if tmpword not in hashbucket or tmp[0] not in pass1dict or tmp[1] not in pass1dict :
                    continue

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


    
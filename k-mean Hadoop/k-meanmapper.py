#!/usr/bin/env python3.4

"""mapper.py"""
import numpy as np
import sys
import ast



distance=[None]*43
sum=[None]*43
centroids=list()

def loadcentroids():
    global centroids
    f=open("lastresult","r")
    for line in f:
        line = line.strip(" \n\r")  
        line=ast.literal_eval(line) 
        centroids.append(np.array(line))

    centroids=np.array(centroids)


def main():

    global distance
    for line in sys.stdin:

        cc+=1    
        line = line.strip(" \n\r")  
        line=ast.literal_eval(line) 

        tmp=np.array(line)
        
        for i in range(centroids.shape[0]):
            distance[i]=np.sum(np.square(centroids[i]-tmp))#calculating distance
        
        index=distance.index(min(distance))# choose the one with least distance
        
        if sum[index]:
            sum[index][0]+=tmp
            sum[index][1]+=1

        else:
            sum[index]=[tmp,1]
        distance=[None]*43
    

def output():
    global sum
    sum=np.array(sum)
    for x in range(centroids.shape[0]):
        if sum[x]:
            print("%s\t%s\t%s"% (str(x),str(sum[x][0].tolist()),str(sum[x][1])) )#gives the partial sum in form of list and total number of the members

loadcentroids()
main()
output()





    
        


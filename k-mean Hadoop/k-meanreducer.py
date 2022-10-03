#!/usr/bin/env python3.4
"""mapper.py"""
import numpy as np
import sys
import ast


centroids=[None]*43
sum=[None]*43
oldcentroids=[]

def loadcentroids():
    global oldcentroids
    f=open("lastresult","r")
    for line in f:
        line = line.strip(" \n\r")  
        line=ast.literal_eval(line) 
        oldcentroids.append(np.array(line))
              
    oldcentroids=np.array(oldcentroids)
    

def main():
    global sum
    for line in sys.stdin:
        line = line.strip(" \n\r") 
        index,matrix,count=line.split("\t")
        index=int(index)
        count=int(count)
        matrix=ast.literal_eval(matrix)

        if sum[index]:
            sum[index][0]+=np.array(matrix)#adding all
            sum[index][1]+=count
        else:
            sum[index]=[np.array(matrix),count]

def newmean():
    global centroids
    global sum 
    global oldcentroids   
    for i in range(43):
        if sum[i]:
            centroids[i]=sum[i][0]/sum[i][1]   #divide it for new mean
        else:
            
            centroids[i]=oldcentroids[i]
            
    #centroids=np.array(centroids)

def output():
    global centroids
    centroids=np.array(centroids)
    for i in range(43):
        print("%s" % str(centroids[i].tolist()))

loadcentroids()
main()
newmean()
output()


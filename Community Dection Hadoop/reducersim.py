#!/usr/bin/env python3.4
"""reducer.py"""


import sys

current_key = None
current_value = None
current_list = []
current_count= 0
current_similarity=0
key = None
value = None



def myFunc(e):
  return e['similarity']

def similarity(key,value,count):
    keyp=0
    valuep=0
    if key in setlist:
      keyp=int(setlist[key])
    if value in setlist:
      valuep=int(setlist[value])
    tmp=keyp+valuep-count

    if tmp==0:
      return 0
    else:
      return (count/tmp)

def printktop(key, current_list):
  k=4
  i=0
  tmp=key+'\t'
  for item in current_list:
    tmp+=item['key']+" "+str(item['similarity'])+" "
    i+=1
    if i>=k:
      break
  print (tmp)




f=open("gpcount","r")

setlist={}


for line in f:
  line = line.strip()


  key , value = line.split("\t")
  setlist[key]=value
  

for line in sys.stdin:

    line = line.strip()

    key , value = line.split("\t")


    if (current_key == key) and (current_value == value):
        current_count+=1
    else:
        if (current_key == key) and (current_value != value):
            current_similarity=similarity(current_key,current_value,current_count)  
            current_list.append({'key' : current_value, 'similarity':current_similarity})
            current_count=1
            current_similarity=0
            current_value=value
        else:
            if current_key:
                current_similarity=similarity(current_key,current_value,current_count)
                current_list.append({'key' : current_value, 'similarity':current_similarity})
                current_list.sort(reverse=True,key=myFunc)
                printktop(current_key,current_list)
            current_value=value
            current_key=key
            current_count=1
            current_list=[]
            current_similarity=0

if current_key == key:
    current_similarity=similarity(current_key,current_value,current_count)
    current_list.append({'key' : current_value, 'similarity':current_similarity})
    current_list.sort(reverse=True,key=myFunc)
    printktop(current_key,current_list)
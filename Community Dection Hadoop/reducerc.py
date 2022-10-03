#!/usr/bin/env python3.4
"""reducer.py"""

from operator import itemgetter
import sys

current_key = None
current_value = None
current_list = []
key = None
value = None

def myFunc(e):
  return e['count']

def printktop(key,key2, current_list):
    tmp=0
    for item in current_list:
        tmp+=int(item)
    
    print ("%s: %s, %s, %s" %(key,key2,current_list,tmp))
  

for line in sys.stdin:

    line = line.strip()

    key,key2 , value = line.split("\t")


    if (current_key == key and current_key2==key2):
        current_list.append(value)
    else:
        if current_key == key:
            printktop(current_key,current_key2,current_list)
            current_key2=key2
            current_list=[]
            current_list.append(value)
        else:
            if current_key:
                printktop(current_key,current_key2,current_list)
            current_key=key
            current_key2=key2
            current_list=[]
            current_list.append(value)

if current_key == key:
    printktop(current_key,current_key2,current_list)
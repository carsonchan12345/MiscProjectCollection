#!/usr/bin/env python3.4
"""reducer.py"""

from operator import itemgetter
import sys

current_key = None
current_value = None
current_list = []
current_count= 0
key = None
value = None

def myFunc(e):
  return e['count']

def printktop(key, current_list):
  k=3
  i=0
  tmp=key+'\t'
  for item in current_list:
    tmp+=item['key']+' '+str(item['count'])+' '
    i+=1
    if i>=k:
      break
  print (tmp)
  

for line in sys.stdin:
   
    line = line.strip()

    key , value = line.split("\t")





    if (current_key == key) and (current_value == value):
        current_count+=1
    else:
        if (current_key == key) and (current_value != value):
            current_list.append({'key' : current_value, 'count':current_count})
            current_count=1
            current_value=value
        else:
            if current_key:
                current_list.append({'key' : current_value, 'count':current_count})
                current_list.sort(reverse=True,key=myFunc)
                printktop(current_key,current_list)
            current_value=value
            current_key=key
            current_count=1
            current_list=[]


if current_key == key:
    current_list.append({'key' : current_value, 'count':current_count})
    current_list.sort(reverse=True,key=myFunc)
    print("%s\t%s"% (current_key,current_list[0]['key']))
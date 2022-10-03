#!/usr/bin/env python3.4

import sys

def myFunc(e):
  return e["count"]

def mergejitter(k,storage):
    
    storage.sort(key=myFunc)
    klargestcount=storage[k-1]["count"]#the 100th largest count 

    for x in storage.copy():
        x["count"]-=klargestcount# minus all with 100th largest count 
        if x["count"]<=0:# see if it is greater than 0
            del x
        



def main():
    k=100
    storage=[]
    current_word = None
    current_count = 0
    word = None

    
    for line in sys.stdin:

        line = line.strip()
        word, count = line.split('\t', 1)

        if current_word == word:
            current_count += count
        else:
            if current_word:
                
                storage.append({"word":current_word,"count":current_count})
            current_count = count
            current_word = word


    if current_word == word:
        storage.append({"word":current_word,"count":current_count})

    for x in storage:
        print("%s\n"% x["word"])
main()
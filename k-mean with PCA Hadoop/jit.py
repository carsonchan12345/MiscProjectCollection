
def jitter(k,storage,words):
    for word in words:
        if (word in storage):#check if it is in the storage
            storage[word]+=1#+1 if exist
        elif (len(storage.keys())<k-1):#check if amount in the list is under k-1 
            storage[word]=1 #create one if not exist
        else:
            for key in storage.copy():#all value in storage -1
                storage[key]-=1
                if storage[key]==0:# if <=0 drop it
                    del storage[key]


def main():
    storage={}
    counter=0
    k=100
    f=open("shakespeare-basket1","r")

    for line in f:
        line = line.strip(" \n\r")
        words = line.split()
        jitter(k,storage,words)#give the array to function


    g=open("jitresult","w")
    for key in storage:
        g.writelines("%s\n"%(key))


main()
    



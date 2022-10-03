import time

start = time.time()
f=open("yelp_review","r")
pass1dict=dict()

def firstpass():
    basketcount=0

    for line in f:
        
        line=line.strip(" \n\t")
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

    




storage=dict()
def secondpass():
    basketcount=0
    f.seek(0)
    for line in f:
        
        basketcount+=1
        
        line=line.strip()
        line=line.split()
        lenline=len(line)
        for x in range(0,lenline):
            for y in range(x,lenline):
                if line[x]==line[y] or line[x] not in pass1dict or line[y] not in pass1dict:
                    continue

                tmp=[line[x],line[y]]
                tmp.sort()
                tmpword=tmp[0]+","+tmp[1]
                if tmpword in storage:
                    storage[tmpword]+=1
                else:
                    storage[tmpword]=1

    threshold=0.005*basketcount
    for k in list(storage.keys()):
        if storage[k]<threshold:
            del storage[k]
    end = time.time()
    print(end - start)
    

def printing():
    d=open("testps2","w")

    for x in storage:
        d.writelines(x+" "+str(storage[x])+"\n")

firstpass()
secondpass()
printing()
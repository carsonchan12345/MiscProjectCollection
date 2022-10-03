ps1file=open("ps1","r")
f=open("yelp_review","r")
ps1=set()
storage=dict()
for line in ps1file:
    line=line.strip("\n")
    ps1.add(line)
ps1file.close()
basketcount=0
print(ps1)
for line in f:
    basketcount+=1
    
    line=line.strip()
    line=line.split()
    lenline=len(line)
    for x in range(0,lenline):
        for y in range(x,lenline):
            if line[x]==line[y] or line[x] not in ps1 or line[y] not in ps1:
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



d=open("ps2","w")

for x in storage:
    d.writelines(x+" "+str(storage[x])+"\n")

    
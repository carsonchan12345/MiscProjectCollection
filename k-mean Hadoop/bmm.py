import gzip
import numpy as np
import ast
import math



def training_images():
    with gzip.open('emnist-letters-train-images-idx3-ubyte.gz', 'r') as f:
        # first 4 bytes is a magic number
        magic_number = int.from_bytes(f.read(4), 'big')
        # second 4 bytes is the number of images
        image_count = int.from_bytes(f.read(4), 'big')
        # third 4 bytes is the row count
        row_count = int.from_bytes(f.read(4), 'big')
        # fourth 4 bytes is the column count
        column_count = int.from_bytes(f.read(4), 'big')
        # rest is the image pixel data, each pixel is stored as an unsigned byte
        # pixel values are 0 to 255
        image_data = f.read(row_count*column_count*image_count)
        images = np.frombuffer(image_data, dtype=np.uint8).astype(np.float32)

        images=images.reshape(image_count, row_count, column_count, order='C')
        
        for i in range(0,image_count):
            b=images[i]>127
            images[i] = b.astype(int)
        
            #z.write(str(images[i].tolist())+"\n")

        return images

def training_labels():#reference to stackoverflow for this mnist loading program
    with open('emnist-letters-train-labels-idx1-ubyte.gz', 'r') as f:
        # first 4 bytes is a magic number
        magic_number = int.from_bytes(f.read(4), 'big')
        # second 4 bytes is the number of labels
        label_count = int.from_bytes(f.read(4), 'big')
        # rest is the label data, each label is stored as unsigned byte
        # label values are 0 to 9
        label_data = f.read()
        labels = np.frombuffer(label_data, dtype=np.uint8)
        return labels



pi=[0]*43
q=[]
y=[ [0]*43 for i in range(124800)]
likelihood=0
f=open("bmm","r")

def loadq():
    global q
    
    for line in f:
        line = line.strip(" \n\r") 
        line=ast.literal_eval(line) 
        q.append(np.array(line))
    q=np.array(q)
#load the q of 0 to 1

    
def updatey():
    global y
    p=[0]*43
    n=0
    total=0
    for line in images:
        
        tmp=np.array(line)
        for k in range(43):
            if p[k]==0:
                p[k]=np.prod((q[k]**tmp)*((1-q[k])**(1-tmp)))
            total+=p[k]*pi[k]
        for k in range(43):
            
            y[n][k]=(pi[k]*p[k])/total#calculating y according to the formula
        p=[0]*43
        n+=1
        total=0#should have update 12800*43 of y

def updatepi():
    global pi
    totaly=0
    for k in range(43):
        for n in range(124800):
            totaly+=y[n][k]
        pi[k]=totaly/12800#calculating pi according to the formula
        totaly=0
        
        
    


def updateq():
    global q

    totaly=0
    total=None

    for k in range(43):
        for n in range(124800):
            if total:
                total+=y[n][k]*images[n]
            else:
                total=y[n][k]*images[n]
            totaly+=y[n][k]
        q[k]=total/totaly#calculating q according to the formula
        total=0
        totaly=0

def updatelikelihood():
    global likelihood
    totalk=0
    totaln=0
    for n in range(124800):
        for k in range(43):
            totalk+=pi[k]*np.prod((q[k]**images[n])*((1-q[k])**(1-images[n])))
        totaln+=math.log(totalk)
        totalk=0#calculating likelihood according to the formula
    print(totaln)
    if totaln==likelihood:
        exit()
    else:
        likelihood=totaln

def initilize():
    global p
    global pi
    sum=[0]*43
    p=[0]*43
    for line in images:

 
        tmp=np.array(line)
        
        for i in range(43):
           
            if p[i]==0:
                p[i]=np.prod((q[i]**tmp)*((1-q[i])**(1-tmp)))

        index=p.index(max(p))

        if sum[index]:
            sum[index]+=1
        else:
            sum[index]=1
        p=[0]*43
    for x in range(43):
        pi[x]=sum[x]/np.sum(sum)
    #initiate the pi

def output():
    g=open("resultq","w")
    global q
    q=np.array(q)
    for i in range(43):
        g.write("%s\n" % (str(q[i].tolist())))   
    



"""
def output():
    g=open("result1","w")
    global centroids
    centroids=np.array(centroids)
    for i in range(43):
        g.write("%s\t%s\n" % (str(i),str(centroids[i].tolist())))   
    """
images=training_images()
loadq()
initilize()


while True:#repeat until converge
    updatey()
    updatepi()
    updateq()
    output()
    updatelikelihood()
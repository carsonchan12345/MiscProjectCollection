import gzip
import numpy as np
import ast
from numba import jit



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
            images[i]=np.transpose(images[i])

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

"""
a=[]
i=0
for line in z:
    line=ast.literal_eval(line)
    a.append(np.array(line))
    i+=1
    if i>1000:
        break
a=np.array(a)
print(i)
while True:
    x=input()
    plt.imshow(images[int(x)])
    plt.show()

"""
round=0
loss=0
centroids=[]
distance=[None]*43
sum=[None]*43
tmpcentroids=[None]*43
f=open("initiatemeanpoint1","r")

def loadcentroids(centroids):
    
    for line in f:
        line = line.strip(" \n\r") 
        #line=line.split("\t") 
        line=ast.literal_eval(line) 
        centroids.append(np.array(line))

@jit

def main():
    newloss=0
    global loss
    global round
    round+=1
    global distance
    global sum
    for line in images:
  
        tmp=np.array(line)
        
        for i in range(43):
            distance[i]=np.sum(np.square(centroids[i]-tmp))
        
        index=distance.index(min(distance))
        newloss+=distance[index]
        if sum[index]:
            sum[index][0]+=tmp
            sum[index][1]+=1
            

        else:
            sum[index]=[tmp,1]
        distance=[None]*43
    sum=np.array(sum)
    
    print(round,newloss)
    """
    for i in range(43):
        if sum[i]:
            print("%s\t%s\t%s"%(i,str(sum[i][0].tolist()),str(sum[i][1])))
            """
    
def newmean():
    global centroids
    global sum 
    global tmpcentroids   
    for i in range(43):
        if sum[i]:
            tmpcentroids[i]=np.multiply(sum[i][0],1/int(sum[i][1]))
        else:
            tmpcentroids[i]=centroids[i]
    tmpcentroids=np.array(tmpcentroids)
    
def stopp():
    global tmpcentroids
    global centroids
    global sum
    c=tmpcentroids==centroids
    if c.all():
        output()
        exit()
    else:
        centroids=tmpcentroids
        tmpcentroids=[None]*43
        sum=[None]*43
            
def output():
    g=open("result1","w")
    global centroids
    centroids=np.array(centroids)
    for i in range(43):
        g.write("%s\t%s\n" % (str(i),str(centroids[i].tolist())))   
    
images=training_images()
loadcentroids()

while True:
    main()
    newmean()
    stopp()
    if (round%10==0):
        output()
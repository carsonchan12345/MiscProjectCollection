import gzip
import numpy as np
import ast
from sklearn.decomposition import PCA
from sklearn.preprocessing import StandardScaler

f=open("pca/result3","r")
centroids=[]
sum=[ [0]*27 for i in range(43)]
p=None
distance=[None]*43

def loadcentroids():
    global centroids
    
    for line in f:
        line = line.strip(" \n\r") 
        line=line.split("\t") 
        line[1]=ast.literal_eval(line[1]) 
        centroids.append(np.array(line[1]))

    centroids=np.array(centroids)
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
        image = np.frombuffer(image_data, dtype=np.uint8).astype(np.float32)

        image=image.reshape(image_count, row_count, column_count, order='C')
        images=np.zeros((image_count,row_count*column_count))
        for i in range(0,image_count):
            tmp=np.transpose(image[i])
            tmp=tmp.flatten()
            images[i]=tmp
            #z.write(str(images[i].tolist())+"\n")

        return images

def testing_images():
    with gzip.open('emnist-letters-test-images-idx3-ubyte.gz', 'r') as f:
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
        image = np.frombuffer(image_data, dtype=np.uint8).astype(np.float32)

        image=image.reshape(image_count, row_count, column_count, order='C')
        images=np.zeros((image_count,row_count*column_count))
        for i in range(0,image_count):
            tmp=np.transpose(image[i])
            tmp=tmp.flatten()
            images[i]=tmp
            #z.write(str(images[i].tolist())+"\n")

        return images

def training_labels():#reference to stackoverflow for this mnist loading program
    with gzip.open('emnist-letters-test-labels-idx1-ubyte.gz', 'r') as f:
        # first 4 bytes is a magic number
        magic_number = int.from_bytes(f.read(4), 'big')
        # second 4 bytes is the number of labels
        label_count = int.from_bytes(f.read(4), 'big')
        # rest is the label data, each label is stored as unsigned byte
        # label values are 0 to 9
        label_data = f.read()
        labels = np.frombuffer(label_data, dtype=np.uint8)
        return labels

def main():
    
    
    round=0
    global centroids
    global distance
    global sum
    global labels
    global images
    global p
    for line in p:
        
        tmp=np.array(line)
        
        for i in range(43):
            distance[i]=np.sum(np.square(centroids[i]-tmp))
        
        index=distance.index(min(distance))
        
        sum[index][labels[round]]+=1

        distance=[None]*43
        round+=1

        #print(i,sum[i].index(max(sum[i])),max(sum[i]))
  

def output():
    g=open("labelresult3test","w")
    global sum
    tmp1=0 #number of data belong to that cluster
    tmp2=0 #number of data correctly identified
    for i in range(43):
        tmp1=np.sum(sum[i])
        tmp2=max(sum[i])
        g.write("%s\t%s\t%s\t%s\t%s\t\n"%(i,tmp1,chr(sum[i].index(max(sum[i]))+64),tmp2,tmp2/tmp1))

def PCAA():
    global p
    global images
    global test
    sc = StandardScaler()
    sc=sc.fit(images)
    images=sc.transform(images)
    test=sc.fit_transform(test)
    pca = PCA(n_components=25).fit(images)
    p=pca.transform(test)
    
    
images=training_images()
test=testing_images()
labels=training_labels()

loadcentroids()
PCAA()
main()
output()


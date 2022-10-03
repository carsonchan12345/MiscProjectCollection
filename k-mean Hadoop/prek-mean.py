import gzip
import numpy as np
import scipy as sp
import ast

g=open("processedimages","r")
z=open("initialkmean","w")

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

images=training_images()


np.random.seed(2000)
k=43
b=[]
i=0
for x in range(k):
    tmp=np.random(784)
    b.append(tmp)
    
b=np.array(b)
for x in range(k):
    z.write(str(b[x].tolist())+"\n")

#the difference is the below code
for x in range(len(images)):
    images[x]=np.array(sp.stats.zscore(images[x]))# here do the standardlization with scipy

r=25
u,s,vt=np.linalg.svd(images,full_matrices=False)#use SVD to do pca after the standardlization
ur = u[:,0:r]
sr = s[0:r]
vtr = vt[0:r,:]

Sigma = np.zeros((25, 25), dtype=float)
Sigma =np.diag(sr[0:25])
y=np.dot(ur,Sigma)#U dot sigma = PCA result
for x in range(len(images)):
    g.write(str(images[x].tolist())+"\n")



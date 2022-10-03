'''Train CIFAR10 with PyTorch.'''
import torch
import torch.nn as nn
import torch.optim as optim
import torch.nn.functional as F
import torch.backends.cudnn as cudnn

import torchvision
import torchvision.transforms as transforms

import os


# Hyper-params (you may change them for better performance)
train_batch_size = 512
lr = 0.01
momentum=0.9
weight_decay=1e-4
transformations = [transforms.RandomCrop(32, padding=4),
                   transforms.RandomHorizontalFlip()]

# Init (do not change them)
num_epoch = 10
device = 'cuda' if torch.cuda.is_available() else 'cpu'
num_workers = 0
print_interval = 10
best_acc = 0  # best test accuracy
start_epoch = 0  # start from epoch 0 or last checkpoint epoch



################################## Data ###################################
print('==> Preparing data..')

transform_train = transforms.Compose(
    transformations + [
    transforms.ToTensor(),
    transforms.Normalize((0.4914, 0.4822, 0.4465), (0.2023, 0.1994, 0.2010)),
])

transform_test = transforms.Compose([
    transforms.ToTensor(),
    transforms.Normalize((0.4914, 0.4822, 0.4465), (0.2023, 0.1994, 0.2010)),
])

trainset = torchvision.datasets.CIFAR10(
    root='./data', train=True, download=True, transform=transform_train)
trainloader = torch.utils.data.DataLoader(
    trainset, batch_size=train_batch_size, shuffle=True, num_workers=num_workers)

testset = torchvision.datasets.CIFAR10(
    root='./data', train=False, download=True, transform=transform_test)
testloader = torch.utils.data.DataLoader(
    testset, batch_size=100, shuffle=False, num_workers=num_workers)

classes = ('plane', 'car', 'bird', 'cat', 'deer',
           'dog', 'frog', 'horse', 'ship', 'truck')



################################## Model ##################################
print('==> Building model..')
class Linear(nn.Module):
    def __init__(self, num_classes=10):
        super(Linear, self).__init__()
        self.fc = nn.Linear(32 * 32 * 3, num_classes)

    def forward(self, x):
        x = x.view(x.size(0), -1)
        x = self.fc(x)
        return x

class MLP(nn.Module):
    def __init__(self, num_classes=10):
        super(MLP, self).__init__()       
        self.fc1 = nn.Linear(32 * 32 * 3, 128)
        self.fc2 = nn.Linear(128, 128)
        self.fc3 = nn.Linear(128, 128)
        self.relu=nn.ReLU()
        self.classifier=nn.Linear(128,num_classes)
        
        

    def forward(self, x):
        x = x.view(x.size(0), -1)
        x=self.fc1(x)
        x=self.relu(x)
        x=self.fc2(x)
        x=self.relu(x)
        x=self.fc3(x)
        x=self.relu(x)
        x=self.classifier(x)
        return x

class LeNet(nn.Module):
    def __init__(self, num_classes=10):
        super(LeNet, self).__init__()
        self.conv1=nn.Conv2d(3, 6, 5)
        self.conv2 = nn.Conv2d(6, 16, 5)
        self.fc1 = nn.Linear(16*5*5, 120)
        self.fc2 = nn.Linear(120, 84)
        self.classifier=nn.Linear(84,num_classes)

    def forward(self, x):
        x = F.max_pool2d(F.relu(self.conv1(x)),(2, 2))
        x = F.max_pool2d(F.relu(self.conv2(x)),(2, 2))
        
        x = x.view(-1, 16*5*5)
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        x=self.classifier(x)
        return x


class AlexNet(nn.Module):
    def __init__(self, num_classes=10):
        super(AlexNet, self).__init__()
        self.conv1=nn.Conv2d(3,64,3,2,1)
        self.conv2=nn.Conv2d(64,192,3,1,1)
        self.conv3=nn.Conv2d(192,384,3,1,1)
        self.conv4=nn.Conv2d(384,256,3,1,1)
        self.conv5=nn.Conv2d(256,256,3,1,1)
        self.dropout=nn.Dropout(0.5)
        self.fc1=nn.Linear(256*2*2,4096)
        self.relu=nn.ReLU()
        self.fc2=nn.Linear(4096,4096)
        self.classifier=nn.Linear(4096,num_classes)

    def forward(self, x):
        x = F.max_pool2d(F.relu(self.conv1(x)),(2, 2))
        
        x = F.max_pool2d(F.relu(self.conv2(x)),(2, 2))
        
        x= F.relu(self.conv3(x))
        x= F.relu(self.conv4(x))
        x= F.max_pool2d(F.relu(self.conv5(x)),(2, 2))
        
        x= self.dropout(x)
        x = x.view(-1, 256*2*2)
        x=self.fc1(x)
        x=self.relu(x)
        x= self.dropout(x)
        x=self.fc2(x)
        x=self.relu(x)
        x=self.classifier(x)
        return x


#net = Linear()
#net = MLP()
net = LeNet()
#net = AlexNet()

net = net.to(device)
if device == 'cuda':
    net = torch.nn.DataParallel(net)
    cudnn.benchmark = True

criterion = nn.CrossEntropyLoss()
optimizer = optim.SGD(net.parameters(), lr=lr,
                      momentum=momentum, weight_decay=weight_decay)
scheduler = torch.optim.lr_scheduler.CosineAnnealingLR(optimizer, T_max=num_epoch)



################################## Train ##################################
def train(epoch):
    net.train()
    train_loss = 0
    correct = 0
    total = 0
    for batch_idx, (inputs, targets) in enumerate(trainloader):
        inputs, targets = inputs.to(device), targets.to(device)
        optimizer.zero_grad()
        outputs = net(inputs)
        loss = criterion(outputs, targets)
        loss.backward()
        optimizer.step()

        train_loss += loss.item()
        _, predicted = outputs.max(1)
        total += targets.size(0)
        correct += predicted.eq(targets).sum().item()

        if batch_idx % print_interval == 0:
            print('Train Epoch: %d | Batch: %d/%d | Loss: %.3f | Train Acc: %.3f%%'
                     % (epoch, batch_idx, len(trainloader),
                     train_loss/(batch_idx+1), 100.*correct/total))


def test(epoch):
    global best_acc
    net.eval()
    test_loss = 0
    correct = 0
    total = 0
    with torch.no_grad():
        for batch_idx, (inputs, targets) in enumerate(testloader):
            inputs, targets = inputs.to(device), targets.to(device)
            outputs = net(inputs)
            loss = criterion(outputs, targets)

            test_loss += loss.item()
            _, predicted = outputs.max(1)
            total += targets.size(0)
            correct += predicted.eq(targets).sum().item()

        print("\n")
        print('Test Epoch: %d | Loss: %.3f | Test Acc: %.3f%%'
                % (epoch, test_loss/(batch_idx+1), 100.*correct/total))
        print("\n")

    # Save checkpoint.
    acc = 100.*correct/total
    if acc > best_acc:
        state = {
            'net': net.state_dict(),
            'acc': acc,
            'epoch': epoch,
        }
        if not os.path.isdir('checkpoint'):
            os.mkdir('checkpoint')
        torch.save(state, './checkpoint/ckpt.pth')
        best_acc = acc


for epoch in range(start_epoch, start_epoch+num_epoch):
    train(epoch)
    test(epoch)
    scheduler.step()

print("Final testing result: %.3f%%" % best_acc)

import os, sys
import matplotlib as plt

f = open('../../logs/exp1.log', 'r')

l = 1

Liter = []
Ltrainloss = []
Lvalloss = []
Lvalacc = []

while(l):
    l = f.readline().strip()
    if 'SGD Iter' in l:
        iter=int(l.split('SGD Iter:')[1].split('trainLoss:')[0])
        trainloss=float(l.split('SGD Iter:')[1].split('trainLoss:')[1])

        Liter.append(iter)
        Ltrainloss.append(trainloss)
    if 'valloss' in l:
        valloss=float(l.split('valloss:')[1].split('valacc:')[0])
        valacc=float(l.split('valloss:')[1].split('valacc:')[1])
        
        Lvalloss.append(valloss)
        Lvalacc.append(valacc)

plt.plot(Liter, trainloss)

Lvaliter = arange(0, Liter[-1], int(float(Liter[-1])/float(len(valacc))))
plt.plot(Lvaliter, valloss, 'r')

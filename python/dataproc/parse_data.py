import os, sys

f = open('../../data/20151102.txt', 'r')
fo = open('../../data/parsed_20151102.txt', 'w')

l=1
while(l):
    l = f.readline().strip()
    if len(l) > 0:
        for a in l.split(',')[4:]:
            fo.write(a+',')
        fo.write('\n')

f.close()
fo.close()

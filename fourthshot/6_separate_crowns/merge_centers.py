#!/usr/bin/env python
import sys
import numpy as np

#Get CSV file name from argument 1
inputfile=sys.argv[1]
#Get cut-off distance from argument 2
distance=float(sys.argv[2])
#Get output csv file from argument 3
outputfile=sys.argv[3]

#Load data to RAM
data=np.genfromtxt(inputfile,delimiter=",",names=True)

def euclidian(xa,ya,xb,yb):
    """
    Euclidian distance of two pairs
    """
    a = np.asarray((xa,ya))
    b = np.asarray((xb,yb))
    d = a - b
    return np.sqrt(( d * d ).sum(-1))

rowtodelete=[]
for i in range(len(data['X'])):
    for j in range(i,len(data['X']),1):
        if(i!=j):
            d=euclidian(data['X'][i],data['Y'][i],data['X'][j],data['Y'][j])
            if (d<distance):
                #print("%.2f: %d,%d"%(d,data['clump'][i],data['clump'][j]))
                data['X'][i]=(data['X'][i]+data['X'][j])/2.0
                data['Y'][i]=(data['Y'][i]+data['Y'][j])/2.0
                #data['radius'][i]=np.asarray(data['radius'][i],data['radius'][j]).max()
                rowtodelete.append(j)

#for i in range(len(rowtodelete),1,1):
r2del=np.unique(np.asarray(rowtodelete))
final_data=np.delete(data,r2del,axis=0)
#np.savetxt(outputfile,final_data,fmt='%d,%f,%f,%f',header="clump,X,Y,radius")
np.savetxt(outputfile,final_data,fmt='%d,%f,%f',header="clump,X,Y")




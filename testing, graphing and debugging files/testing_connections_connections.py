#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Nov 24 14:47:16 2019

@author: launy
"""

import pandas as pd
import glob
from time import perf_counter

list=glob.glob('*connections*.txt')
list.sort()


start = perf_counter()  
for ele in range(size(list)):
    y=0 #will count how many total connections are valid
    df=pd.read_csv(list[ele],index_col=False,nrows=100) # inport file to data fram
    #df=df.drop('Unnamed: 0',axis=1) # cleans the dataframe
    df=df.fillna(-1) #makes NA's -1 for ease of comparison later
    #df=df.astype(int) # turns float to int 
    arr=df.to_numpy() # makes dataframe values to Numpy array, faster than using dataframe
     
    for i in range(50):
        x=0 #will count number of individual rows of correct connections must be 6
        for j in range(2,8):
            if(arr[i][j]>=0 and arr[i][j-1]!=arr[i][j]):
                x+=1
        if(x!=6):
            print(ele,i)
        if(x==6):
            y+=1
    if(y!=50):
        print(ele)
        break
end = perf_counter()
execution_time = (end - start)
print(execution_time)

#!/usr/bin/env python3
import matplotlib.pyplot as plt 
import pandas as pd 
import glob 
from matplotlib.ticker import FuncFormatter, MaxNLocator 
import numpy as np 
  
list=glob.glob("NK_space_[0-9]*.txt") 
for i in list: 
	df=pd.read_csv(i) 
	rounds=df["round"].tolist() 
	avgscores=df["avg score"].to_list() 
	maxscores=df["max score"].to_list() 
	plt.plot(rounds,avgscores,label='Average Score') 
	plt.xlabel("Number of Rounds") 
	plt.plot(rounds,maxscores,label='Max Score')
	plt.ylabel("Score") 
	plt.axis([-.9, rounds[-1]+1,min(avgscores)-0.1,max(maxscores)+.05]) 
	plt.xticks(range(0, rounds[-1], 1)) 
	plt.yticks(np.arange(.5,1,step=.05)) 
	plt.legend() 
	plt.savefig(i[:-4]+".png") 
	plt.close() 

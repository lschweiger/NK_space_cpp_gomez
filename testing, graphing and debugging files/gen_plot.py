import matplotlib.pyplot as plt
import glob
import csv
import gc

list=glob.glob('*scores_1_500_*R.txt')
list.append(glob.glob('*scores_5_635_*R.txt')[0])
list.append(glob.glob('*scores_10_665_*R.txt')[0])

def plots(files):
	for i in files:
	    x=[]
	    y=[]
	    with open(i) as csvfile:
	        data=csv.reader(csvfile)
	        num=0
	        for row in data:
	            x.append(num)
	            y.append(float(row[0]))
	            num+=1
	    plt.figure(figsize=(19.8,18.2))
	    plt.plot(x,y,linewidth=1)
	    plt.xlabel(i)
	    plt.savefig(i+"_plot.png")
	    #plt.show()
	    plt.close("all")
	    x=0
	    y=0
	    gc.collect()

def scatters(files):
	for i in files:
	    x=[]
	    y=[]
	    with open(i) as csvfile:
	        data=csv.reader(csvfile)
	        num=0
	        for row in data:
	            x.append(num)
	            y.append(float(row[0]))
	            num+=1
	    plt.figure(figsize=(19.8,18.2))
	    plt.scatter(x,y,linewidth=1)
	    plt.xlabel(i)
	    plt.savefig(i+"_scatter.png")
	    #plt.show()
	    plt.close("all")
	    x=0
	    y=0
	    gc.collect()


plots(list)
scatters(list)
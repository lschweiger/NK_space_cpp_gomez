#!/usr/bin/env python3
import sys
import bz2
import lzma
import argparse

parser = argparse.ArgumentParser(description='Input number for compression')
parser.add_argument('-N', help='number for "For loop"')
args = parser.parse_args()
from shutil import copyfileobj
for i in range(int(args.N)):
	with open('NK_space_scores_'+str(i)+'.txt', 'rb') as input:
		filename = 'NK_space_scores_' + str(i)+".txt"
		with bz2.BZ2File(filename+'.bz2', 'wb', compresslevel=9) as output:
			copyfileobj(input, output)
		#with lzma.open(filename+".xz", "w") as f:
		#	copyfileobj(input,f)
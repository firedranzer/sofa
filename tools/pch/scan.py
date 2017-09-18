#!/usr/bin/python
import os
import sys

path=sys.argv[1]

# traverse root directory, and list directories as dirs and files as files
for root, dirs, files in os.walk(path):
    path = root.split(os.sep)
    #print((len(path) - 1) * '---', os.path.basename(root))
    for file in files:
    	if file.endswith(".h"):
	        #print(len(path) * '---', file)
	        if not root.startswith("./frame"):
	        	print("include<"+root[2:]+os.sep+file+">")

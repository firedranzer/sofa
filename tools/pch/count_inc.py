#!/usr/bin/python
import os
import sys

count = {}

for line in sys.stdin:
	index = line.find('/')
	if index >= 0:
		if not line.endswith(".so") and not line.endswith(".cpp") and not line.endswith(".o") :
			include = line[index:-1]
			if not include in count:
				count[include] = 0
			count[include]+=1
			

print("<html>")
print("<body>")
print("Statistics: <br>")
print("Number of include loaded: "+str(len(count))+" <br>")
print("<ul>") 
for k in sorted(count.keys()):
	print("<li>"+k+" loaded "+str(count[k])+"</li>")
print("</ul>") 
print("</body>")
print("</html>")


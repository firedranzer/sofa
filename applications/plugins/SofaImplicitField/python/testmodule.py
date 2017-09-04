
# coding=utf-

def gradFunction(x,y,z):
	return [0.0, 0.0, 0.0]

def gradFunctionInvalid1(x,y,z,c):
	return [0.0, 0.0, 0.0]

def gradFunctionInvalid2(x,y):
	return [0.0, 0.0, 0.0]

def gradFunctionInvalid3(x,y,z):
	return None

def gradFunctionInvalid4(x,y,z):
	return [0.0,0.0,None]

def zeroFunction(x,y,z):
	return 0.0

def oneFunction(x,y,z):
	return 1.0

def xFunction(x,y,z):
	return x 

def yFunction(x,y,z):
	return y

def zFunction(x,y,z):
	return z 

def sphereFunction(x,y,z):
	return -1.0 

def sphereGradFunction(x,y,z):
	return (1.0, 2.0, 3.0) 


notAFunction = 5

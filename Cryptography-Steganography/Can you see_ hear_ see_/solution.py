#!/usr/bin/python3

with open('encode.txt','r') as file:
	data = file.read()

data = data.replace("#", "sJ")

data = data.replace(chr(33,"s")

data = data.replace("G ",chr(71))

with open('delta.txt', 'w') as f:

	    f.write(data)
#!/usr/bin/env python
#
# icap-client.py
# Copyright (c) 2013 Uditha Atukorala
#

# simple icap client to test a server implementation

import socket
import sys

HOST    = 'localhost'
SERVICE = 'icap://icap.server.net/sample-service'
PORT    = 1344

try:
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
except socket.error, msg:
	sys.stderr.write("[ERROR] %s\n" % msg[1])
	sys.exit(1)

try:
	sock.connect((HOST, PORT))
except socket.error, msg:
	sys.stderr.write("[ERROR] %s\n" % msg[1])
	sys.exit(2)

sock.send( "REQMOD %s ICAP/1.0\r\n" % ( SERVICE ) )
sock.send( "Host: %s\r\n\r\n" % ( HOST ) )

data = sock.recv(1024)
string = ""
while len(data):
	string = string + data
	data = sock.recv(1024)
sock.close()

print string

sys.exit(0)


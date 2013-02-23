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
sock.send( "Host: %s\r\n" % ( HOST ) )
sock.send( "Encapsulated: req-hdr=0, null-body=170\r\n" )
sock.send( "\r\n" )

sock.send( "GET / HTTP/1.1\r\n" )
sock.send( "Host: www.origin-server.com\r\n" )
sock.send( "Accept: text/html, text/plain\r\n" )
sock.send( "Accept-Encoding: compress\r\n" )
sock.send( "Cookie: ff39fk3jur@4ii0e02i\r\n" )
sock.send( "If-None-Match: \"xyzzy\", \"r2d2xxxe\"\r\n" )
sock.send( "\r\n" )


data = sock.recv(1024)
string = ""
while len(data):
	string = string + data
	data = sock.recv(1024)
sock.close()

print string

# POST
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
sock.send( "Host: %s\r\n" % ( HOST ) )
sock.send( "Encapsulated: req-hdr=0, req-body=147\r\n" )
sock.send( "\r\n" )

sock.send( "POST /origin-resource/form.pl HTTP/1.1\r\n" )
sock.send( "Host: www.origin-server.com\r\n" )
sock.send( "Accept: text/html, text/plain\r\n" )
sock.send( "Accept-Encoding: compress\r\n" )
sock.send( "Pragma: no-cache\r\n" )
sock.send( "\r\n" )
sock.send( "1e\r\n" )
sock.send( "I am posting this information.\r\n" )
sock.send( "0\r\n" )
sock.send( "\r\n" )

data = sock.recv(1024)
string = ""
while len(data):
	string = string + data
	data = sock.recv(1024)
sock.close()

print string


sys.exit(0)


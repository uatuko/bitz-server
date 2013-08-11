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

# OPTIONS
print "----- OPTIONS -----"
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

sock.send( "OPTIONS %s ICAP/1.0\r\n" % ( SERVICE ) )
sock.send( "Host: %s\r\n" % ( HOST ) )
sock.send( "User-Agent: Python ICAP tester\r\n" )
sock.send( "\r\n" )


data = sock.recv(1024)
string = ""
while len(data):
	string = string + data
	data = sock.recv(1024)
sock.close()

print string


# REQMOD, GET
print "----- REQMOD - GET -----"
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


# REQMOD, POST
print "----- REQMOD - POST -----"
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


# REQMOD - Message preview
print "----- REQMOD - Message preview -----"
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
sock.send( "Preview: 1024\r\n" )
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
sock.send( "0; ieof\r\n\r\n" )

data = sock.recv(1024)
string = ""
while len(data):
	string = string + data
	data = sock.recv(1024)
sock.close()

print string


# RESPMOD
print "----- RESPMOD -----"
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

sock.send( "RESPMOD %s ICAP/1.0\r\n" % ( SERVICE ) )
sock.send( "Host: %s\r\n" % ( HOST ) )
sock.send( "Encapsulated: req-hdr=0, res-hdr=137, res-body=296\r\n" )
sock.send( "\r\n" )

sock.send( "GET /origin-resource HTTP/1.1\r\n" )
sock.send( "Host: www.origin-server.com\r\n" )
sock.send( "Accept: text/html, text/plain, image/gif\r\n" )
sock.send( "Accept-Encoding: gzip, compress\r\n" )
sock.send( "\r\n" )
sock.send( "HTTP/1.1 200 OK\r\n" )
sock.send( "Date: Mon, 10 Jan 2000 09:52:22 GMT\r\n" )
sock.send( "Server: Apache/1.3.6 (Unix)\r\n" )
sock.send( 'ETag: "63840-1ab7-378d415b"\r\n' )
sock.send( "Content-Type: text/html\r\n" )
sock.send( "Content-Length: 51\r\n" )
sock.send( "\r\n" )
sock.send( "33\r\n" )
sock.send( "This is data that was returned by an origin server.\r\n" )
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


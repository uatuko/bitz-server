#
# modpy.py
# Copyright (c) 2013 Uditha Atukorala
#

import bitz

def init():
	print "init() called"

def cleanup():
	print "cleanup() called"
	
def preview():
	print "preview() called"
	
def modify( request ):
	print "modify() called: ", request
	print "numargs: ", bitz.numargs()


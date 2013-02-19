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
	request = bitz.get_request( request )
	print "payload: \r\n", request['payload']

	response = bitz.get_response_from_status( 501 );
	return response;


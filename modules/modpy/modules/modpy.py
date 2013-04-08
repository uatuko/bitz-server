#
# modpy.py
# Copyright (c) 2013 Uditha Atukorala
#

import bitz

def init():
	print "init() called";

def cleanup():
	print "cleanup() called";

def preview():
	print "preview() called";

def modify( request ):
	request     = bitz.get_request( request );
	req_payload = request['payload'];
	print "payload: \r\n", req_payload;

	# response
	resp_payload = {};
	resp_payload['req_header'] = req_payload['req_header'];
	resp_payload['req_body']   = req_payload['req_body'];
	resp_payload['res_header'] = req_payload['res_header'];
	resp_payload['res_body']   = req_payload['res_body'];

	response = bitz.get_response( 200, resp_payload );
	return response;


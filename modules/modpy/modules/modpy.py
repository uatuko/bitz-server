#
# modpy.py
# Copyright (c) 2013 Uditha Atukorala
#

import bitz

def init():
	print( "init() called" );

def cleanup():
	print( "cleanup() called" );

def preview( request ):
	request     = bitz.get_request( request );
	req_payload = request['payload'];
	print( "preview payload: \r\n", req_payload );

	# response
	if req_payload['ieof']:
		response = bitz.get_response_from_status( 204 );
	else:
		response = bitz.get_response_from_status( 100 );

	return response;

def modify( request ):
	request     = bitz.get_request( request );
	req_payload = request['payload'];
	print( "modify payload: \r\n", req_payload );

	# response
	resp_payload = {};
	if request['request'] == 'REQMOD':
		resp_payload['req_header'] = req_payload['req_header'];
		resp_payload['req_body']   = req_payload['req_body'];
	else:
		resp_payload['req_header'] = '';
		resp_payload['req_body']   = '';

	resp_payload['res_header'] = req_payload['res_header'];
	resp_payload['res_body']   = req_payload['res_body'];
	resp_payload['ieof']       = True;

	response = bitz.get_response( 200, resp_payload );
	return response;


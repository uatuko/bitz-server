/*
 *	bitz-server, An ICAP server implementation in C++
 *	Copyright (C) 2012	Uditha Atukorala
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "interface.h"

#include <bitz/logger.h>
#include <icap/request.h>
#include <icap/response.h>
#include <iostream>


PyObject * bitz_get_request( PyObject * self, PyObject * pyrequest ) {

	PyObject * pyreturn;
	PyObject * pypayload;
	icap::Request * request;

	// logger
	bitz::Logger &logger = bitz::Logger::instance();
	logger.debug( "[modpy.interface] get_request()" );

	// initialise return dictionary
	pyreturn = PyDict_New();

	// grab the request object pointer from args
	void * p = PyCapsule_GetPointer( pyrequest, "request" );

	// sanity check
	if ( p != NULL ) {

		// construct the request
		request = static_cast<icap::Request *>(p);

		PyDict_SetItemString( pyreturn, "request", PyString_FromString( request->header()->method().c_str() ) );
		PyDict_SetItemString( pyreturn, "uri", PyString_FromString( request->header()->uri().c_str() ) );
		PyDict_SetItemString( pyreturn, "protocol", PyString_FromString( request->header()->protocol().c_str() ) );

		// payload dictionary
		pypayload = PyDict_New();
		PyDict_SetItemString( pypayload, "req_header", PyString_FromStringAndSize( request->payload().req_header.c_str(), request->payload().req_header.size() ) );
		PyDict_SetItemString( pypayload, "req_body", PyString_FromStringAndSize( request->payload().req_body.c_str(), request->payload().req_body.size() ) );
		PyDict_SetItemString( pypayload, "res_header", PyString_FromStringAndSize( request->payload().res_header.c_str(), request->payload().res_header.size() ) );
		PyDict_SetItemString( pypayload, "res_body", PyString_FromStringAndSize( request->payload().res_body.c_str(), request->payload().res_body.size() ) );
		PyDict_SetItemString( pypayload, "ieof", PyBool_FromLong( request->payload().ieof ) );

		PyDict_SetItemString( pyreturn, "payload", pypayload );

		// cleanup
		Py_DECREF( pypayload );

	} else {
		logger.warn( "[modpy.interface] failed to get request object pointer" );
	}

	return pyreturn;

}


PyObject * bitz_get_response_from_status( PyObject * self, PyObject * args ) {

	PyObject * pyresponse;
	icap::Response * response;

	unsigned int resp_status;

	// logger
	bitz::Logger &logger = bitz::Logger::instance();
	logger.debug( "[modpy.interface] get_response_from_status()" );

	// parse args
	if ( PyArg_ParseTuple( args, "I", &resp_status ) ) {
		response = new icap::Response( (icap::ResponseHeader::status_t) resp_status );
	} else {
		logger.warn( "[modpy.interface] failed to parse arguments" );
		response = new icap::Response( icap::ResponseHeader::SERVER_ERROR );
	}

	// convert the response into a capsule
	pyresponse = PyCapsule_New( (void *) response, "response", NULL );

	return pyresponse;

}


PyObject * bitz_get_response( PyObject * self, PyObject * args ) {

	PyObject * pyresponse;
	PyObject * pypayload;
	icap::Response * response = NULL;

	unsigned int resp_status;
	icap::payload_t payload;

	// vars to ferry across strings from python dictionary to c++
	char * pybuffer;
	Py_ssize_t pybuflen;


	// logger
	bitz::Logger &logger = bitz::Logger::instance();
	logger.debug( "[modpy.interface] get_response()" );

	// parse args
	if ( PyArg_ParseTuple( args, "IO!", &resp_status, &PyDict_Type, &pypayload ) ) {

		// copy strings from python dictionary
		PyString_AsStringAndSize( PyDict_GetItemString( pypayload, "req_header" ), &pybuffer, &pybuflen );
		payload.req_header.assign( pybuffer, pybuflen );

		PyString_AsStringAndSize( PyDict_GetItemString( pypayload, "req_body" ), &pybuffer, &pybuflen );
		payload.req_body.assign( pybuffer, pybuflen );

		PyString_AsStringAndSize( PyDict_GetItemString( pypayload, "res_header" ), &pybuffer, &pybuflen );
		payload.res_header.assign( pybuffer, pybuflen );

		PyString_AsStringAndSize( PyDict_GetItemString( pypayload, "res_body" ), &pybuffer, &pybuflen );
		payload.res_body.assign( pybuffer, pybuflen );

		// copy other data types from python dictionary
		payload.ieof = PyBool_Check( PyDict_GetItemString( pypayload, "ieof" ) );

		// construct the response object
		response = new icap::Response( (icap::ResponseHeader::status_t) resp_status );
		response->payload( payload );

	} else {
		logger.warn( "[modpy.interface] failed to parse arguments" );
	}

	// sanity check
	if ( response == NULL ) {
		response = new icap::Response( icap::ResponseHeader::SERVER_ERROR );
	}

	// convert the response into a capsule
	pyresponse = PyCapsule_New( (void *) response, "response", NULL );

	return pyresponse;

}



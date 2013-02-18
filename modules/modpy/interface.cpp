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


PyObject * bitz_get_request( PyObject * self, PyObject * args ) {

	PyObject * pyrequest;
	PyObject * pyreturn;

	icap::Request  * request;

	// logger
	bitz::Logger &logger = bitz::Logger::instance();
	logger.debug( "[modpy.interface] get_request()" );

	// initialise return dictionary
	pyreturn = PyDict_New();

	if ( PyArg_ParseTuple( args, "O", &pyrequest ) ) {

		// grab the request object pointer from args
		void * p = PyCapsule_GetPointer( pyrequest, "request" );

		// sanity check
		if ( p != NULL ) {

			// construct the request
			request = static_cast<icap::Request *>(p);

			PyDict_SetItemString( pyreturn, "request", PyString_FromString( request->header()->method().c_str() ) );
			PyDict_SetItemString( pyreturn, "uri", PyString_FromString( request->header()->uri().c_str() ) );
			PyDict_SetItemString( pyreturn, "protocol", PyString_FromString( request->header()->protocol().c_str() ) );
			PyDict_SetItemString( pyreturn, "payload", PyString_FromString( request->payload().c_str() ) );

		} else {
			logger.warn( "[modpy.interface] failed to get request object pointer" );
		}

	} else {
		logger.warn( "[modpy.interface] failed to parse arguments" );
	}

	return pyreturn;

}


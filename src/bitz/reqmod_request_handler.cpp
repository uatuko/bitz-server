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

#include "reqmod_request_handler.h"
#include "request_modifier.h"

#include <dlfcn.h>
#include <iostream>


namespace bitz {

	ReqmodRequestHandler::ReqmodRequestHandler() : RequestHandler( "REQMOD" ) { }
	ReqmodRequestHandler::~ReqmodRequestHandler() { }

	icap::Response * ReqmodRequestHandler::process( icap::RequestHeader * req_header, socketlibrary::TCPSocket * socket ) throw() {

		// TODO
		/*
		void * module = dlopen( "/tmp/root/lib/libreqmod.dylib", RTLD_LAZY);
	    if (! module ) {
	        std::cerr << "Cannot load library: " << dlerror() << std::endl;
	    }

	    // reset errors
	    dlerror();

		reqmod_create_t * create_module   = ( reqmod_create_t * ) dlsym( module, "create" );
	    const char * dlsym_error = dlerror();
	    if ( dlsym_error ) {
	        std::cerr << "Cannot load symbol create: " << dlsym_error << std::endl;
	    } else {
	    	std::cout << "create symbol loaded!" << std::endl;
	    }

		reqmod_destroy_t * destroy_module = ( reqmod_destroy_t * ) dlsym( module, "destroy" );
	    dlsym_error = dlerror();
	    if ( dlsym_error ) {
	        std::cerr << "Cannot load symbol destroy: " << dlsym_error << std::endl;
	    } else {
	    	std::cout << "destroy symbol loaded!" << std::endl;
	    }*/

		/*
		 * notes:
		 *    + read the remaining data and construct the request object
		 *    + check for preview
		 *        - if preview, user the preview() from the module to get the response
		 *        - if the response is "100 continue" send it back to the client here and
		 *          then read and append to the request obj and use modify() to get the response
		 *    + if not in preview use the modify() to get the response and return
		 *
		 */
		icap::Response * response = new icap::Response( icap::ResponseHeader::SERVER_ERROR );
		return response;

	}

} /* end of namespace bitz */


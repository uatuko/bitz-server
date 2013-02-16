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


namespace bitz {

	ReqmodRequestHandler::ReqmodRequestHandler() : RequestHandler( "REQMOD" ) { }
	ReqmodRequestHandler::~ReqmodRequestHandler() { }

	icap::Response * ReqmodRequestHandler::process( icap::RequestHeader * req_header, socketlibrary::TCPSocket * socket ) throw() {

		Modifier::symbols_t symbols;
		icap::Request  * request;
		icap::Response * response;

		// FIXME: we shouldn't be loading and unloading the modules for each request
		// FIXME: read module info from configs
		// FIXME: error handling
		// dynamic loading
		load_modifier( "/tmp/root/etc/bitz/modules/mod_py.so", symbols );

		// TODO: read the request
		request = new icap::Request( req_header );

		// modify
		Modifier * modifier = symbols.create();
		response = modifier->modify( request );
		symbols.destroy( modifier );

		// unload
		unload_modifier( symbols.modifier );

		/*
		 * TODO notes:
		 *    + read the remaining data and construct the request object
		 *    + check for preview
		 *        - if preview, user the preview() from the module to get the response
		 *        - if the response is "100 continue" send it back to the client here and
		 *          then read and append to the request obj and use modify() to get the response
		 *    + if not in preview use the modify() to get the response and return
		 *
		 */

		// cleanup
		delete request;

		return response;

	}

} /* end of namespace bitz */


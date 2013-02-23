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

#include "options_request_handler.h"


namespace bitz {

	OptionsRequestHandler::OptionsRequestHandler() : RequestHandler( "OPTIONS" ) { }
	OptionsRequestHandler::~OptionsRequestHandler() { }


	icap::Response * OptionsRequestHandler::process( icap::RequestHeader * req_header, socketlibrary::TCPSocket * socket ) throw() {

		icap::ResponseHeader * header;
		icap::Response       * response;

		// header
		header = new icap::ResponseHeader( icap::ResponseHeader::OK );

		// FIXME: Methods are tied to the URIs
		header->attach( "Methods", _methods.at( 0 ) );
		header->attach( "Options-TTL", "3600" );
		header->attach( "Allow", "204" );

		response = new icap::Response( header );
		return response;

	}


	void OptionsRequestHandler::register_handler( RequestHandler * req_handler ) throw() {

		// FIXME: Methods are tied to the URIs
		_methods.push_back( req_handler->method() );
		return;

	}

} /* end of namespace bitz */


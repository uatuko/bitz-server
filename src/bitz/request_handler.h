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

#ifndef BITZ_REQUEST_HANDLER_H
#define BITZ_REQUEST_HANDLER_H

#include "modifier.h"

#include <icap/response.h>
#include <icap/request.h>
#include <socket/socket.h>


namespace bitz {

	class RequestHandler {
	public:

		struct req_handler_t {
			std::string method;
		};

		RequestHandler( const std::string &method );
		virtual ~RequestHandler();

		/**
		*   Returns the request method handled by this handler
		*   @return method
		*/
		const std::string &method() const throw();

		/**
		*   Process the request and return a new response object. This will also
		*   read from the passed in socket if more data needs to be read.
		*
		*   @param req_header request header object
		*   @param socket socket object to read the data from
		*   @return response object
		*/
		virtual icap::Response * process( icap::RequestHeader * req_header, socketlibrary::TCPSocket * socket ) throw() =0;


	protected:

		/**
		*   Load a modifier module
		*
		*   @param file file name / path of the module
		*   @param symbols structure to return the symbols
		*   @return boolean to denote success or failure
		*/
		bool load_modifier( const std::string &file, Modifier::symbols_t &symbols ) throw();

		/**
		*   Unload a modifier module
		*
		*   @param modifier pointer to the modifier to unload
		*/
		void unload_modifier( void * modifier ) throw();


	private:

		req_handler_t _req_handler;

	};

} /* end of namespace bitz */

#endif /* !BITZ_REQUEST_HANDLER_H */


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
#include <psocksxx/iosockstream.h>


namespace bitz {

	class RequestHandler {
	public:

		struct req_handler_t {
			std::string method;
		};

		struct handler_t {
			std::string name;
			Modifier::symbols_t symbols;
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
		virtual icap::Response * process( icap::RequestHeader * req_header, psocksxx::iosockstream * socket ) throw();


	protected:

		unsigned int _handlers_count;
		handler_t * _handlers;

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

		/**
		*   Load all the configured modifier modules for this request handler
		*/
		void load_modules() throw();

		/**
		*  Cleanup all the loaded modifier modules
		*/
		void cleanup_modules() throw();

		/**
		*   Given a request instance and a socket instance to communicate, this method will use the
		*   loaded handler modules to grab a preview response. This will return a icap::Response
		*   object or NULL after processing a '100 Continue' response.
		*
		*   @param request request object
		*   @param socket socket object to read data from
		*   @return preview response (response object)
		*/
		icap::Response * process_preview( icap::Request * request, psocksxx::iosockstream * socket ) throw();

		/**
		*   This method will use the loaded handler modules to get a response to the request.
		*
		*   @param request request object
		*   @return response object
		*/
		icap::Response * process_modify( icap::Request * request ) throw();

		/**
		*   Helper method to set a '100 Continue' response back to the client and read the full request.
		*
		*   @param response response object with status 100
		*   @param request request object
		*   @param socket socket object to read / write data
		*   @return
		*/
		bool preview_continue( icap::Response * response, icap::Request * request, psocksxx::iosockstream * socket ) throw();

	private:

		req_handler_t _req_handler;

	};

} /* end of namespace bitz */

#endif /* !BITZ_REQUEST_HANDLER_H */


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

#ifndef BITZ_REQMOD_REQUEST_HANDLER_H
#define BITZ_REQMOD_REQUEST_HANDLER_H

#include "request_handler.h"


namespace bitz {

	class ReqmodRequestHandler : public RequestHandler {
	public:

		struct handler_t {
			std::string name;
			Modifier::symbols_t symbols;
		};

		ReqmodRequestHandler();
		virtual ~ReqmodRequestHandler();

		/**
		*   Process the ICAP request and return a response object which can
		*   be used to send the final response back to the client.
		*
		*   @param req_header request header
		*   @param socket communication socket
		*   @return response object
		*/
		icap::Response * process( icap::RequestHeader * req_header, socketlibrary::TCPSocket * socket ) throw();

	private:
		unsigned int _handlers_count;
		handler_t * _handlers;

		void load_modules() throw();
		void cleanup_modules() throw();

		icap::Response * process_preview( icap::Request * request, socketlibrary::TCPSocket * socket ) throw();
		icap::Response * process_modify( icap::Request * request ) throw();

		bool preview_continue( icap::Response * response, icap::Request * request, socketlibrary::TCPSocket * socket ) throw();

	};

} /* end of namespace bitz */

#endif /* !BITZ_REQMOD_REQUEST_HANDLER_H */


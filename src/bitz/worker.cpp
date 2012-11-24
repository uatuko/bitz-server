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

#include "worker.h"
#include "logger.h"
#include "util.h"

#include <icap/util.h>
#include <icap/request_header.h>


namespace bitz {

	Worker::Worker() {}
	Worker::~Worker() {
		Logger &logger = Logger::instance();
		logger.debug( "exiting worker" );
	}

	void Worker::run( socketlibrary::TCPServerSocket * server_sock, const req_handlers_t &req_handlers, unsigned int max_requests ) throw() {

		Logger &logger = Logger::instance();

		socketlibrary::TCPSocket * client_sock;
		icap::RequestHeader * req_header;
		icap::Response * response;
		RequestHandler * req_handler;


		try {

			while ( max_requests > 0 ) {

				logger.debug( std::string( "[worker] waiting for a connection" ) );

				client_sock = server_sock->accept();
				logger.debug( std::string( "[worker] new connection accepted on " ).append( client_sock->getForeignAddress() )
						.append( ":" ).append( util::itoa( client_sock->getForeignPort() ) ) );

				req_header  = icap::util::read_req_header( client_sock );
				req_handler = util::find_req_handler( req_handlers, req_header->method() );

				if ( req_handler != NULL ) {

					logger.debug( std::string( "[worker] handling request: " ).append( req_header->method() ) );

					// process the request and grab the response
					response = req_handler->process( req_header, client_sock );

				} else {

					// unsupported request
					logger.info( std::string( "[worker] unsupported request: " ).append( req_header->method() ) );
					response = new icap::Response( new icap::ResponseHeader( icap::ResponseHeader::NOT_ALLOWED ) );

				}

				// send the response back to the client
				icap::util::send_response( response, client_sock );

				// cleanup
				delete response;
				delete req_header;

				// destroy / close connection
				delete client_sock;

				max_requests--;

			}

		} catch( socketlibrary::SocketException &sex ) {
			logger.error( std::string( "[worker] ERROR: " ).append( sex.what() ) );
		}

	}

} /* end of namespace bitz */


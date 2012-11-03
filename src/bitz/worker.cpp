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

#include <icap/util.h>
#include <icap/request_header.h>

namespace bitz {

	Worker::Worker() {}
	Worker::~Worker() {
		Logger &logger = Logger::instance();
		logger.debug( "exiting worker" );
	}

	void Worker::run( socketlibrary::TCPServerSocket * server_sock, req_handlers_t req_handlers, unsigned int max_requests ) throw() {

		socketlibrary::TCPSocket * client_sock;
		icap::RequestHeader * req_header;
		RequestHandler * req_handler;

		req_handlers_index_t rh_i;


		try {

			while ( max_requests > 0 ) {

				client_sock = server_sock->accept();
				std::cout << "New connection accepted on " << client_sock->getForeignAddress() << ":" << client_sock->getForeignPort() << std::endl;

				req_header = icap::util::read_req_header( client_sock );
				rh_i = req_handlers.find( req_header->method() );

				if ( rh_i != req_handlers.end() ) {

					req_handler = rh_i->second;

					// TODO: handler the request

				} else {
					// TODO: unsupported request
				}

				// cleanup
				delete req_header;

				// destroy / close connection
				delete client_sock;

				max_requests--;

			}

		} catch( socketlibrary::SocketException &sex ) {
			std::cout << "ERROR: " << sex.what() << std::endl;
		}

	}

} /* end of namespace bitz */


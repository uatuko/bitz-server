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

namespace bitz {

	Worker::Worker() {}
	Worker::~Worker() {
		Logger &logger = Logger::instance();
		logger.debug( "exiting worker" );
	}

	void Worker::run( socketlibrary::TCPServerSocket * server_sock, unsigned int max_requests ) throw() {

		socketlibrary::TCPSocket * client_sock;

		int  line_len;
		char line[1024];

		try {

			while ( max_requests > 0 ) {

				client_sock = server_sock->accept();
				std::cout << "New connection accepted on " << client_sock->getForeignAddress() << ":" << client_sock->getForeignPort() << std::endl;

				// read
				line_len = client_sock->readline( line, 1024 );
				if ( line_len == -1) {
					std::cout << "Failed to read from connection" << std::endl;
				} else {

					std::cout << "client said: " << line << std::endl;

					// echo back
					client_sock->send( line, line_len );

				}

				// destroy / close connection
				delete client_sock;

				max_requests--;

			}

		} catch( socketlibrary::SocketException &sex ) {
			std::cout << "ERROR: " << sex.what() << std::endl;
		}

	}

} // end of namespace bitz


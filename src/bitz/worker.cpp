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
#include "options_request_handler.h"
#include "reqmod_request_handler.h"
#include "respmod_request_handler.h"

#include <icap/util.h>
#include <icap/request_header.h>


namespace bitz {

	Worker::Worker() {

		// load request handlers
		load_req_handlers();

	}


	Worker::~Worker() {

		// logger
		Logger &logger = Logger::instance();
		logger.debug( "[worker] exiting" );

		// cleanup request handlers
		util::delete_req_handlers( _req_handlers );
		delete _req_handlers["OPTIONS"];

	}


	void Worker::run( psocksxx::tcpnsockstream * server_sock, unsigned int max_requests,
			unsigned int comm_timeout ) throw() {

		// logger
		Logger &logger = Logger::instance();

		// client socket stream
		psocksxx::nsockstream * client_sock;

		while ( max_requests > 0 ) {

			logger.debug( std::string( "[worker] waiting for a connection" ) );

			try {

				// accept a client connection
				client_sock = server_sock->accept();

				// FIXME: log accepted client details (e.g. address, port etc.)
				logger.debug( std::string( "[worker] new connection accepted on [...]" ) );

				// set timeout value
				if ( comm_timeout > 0 ) {
					client_sock->timeout( comm_timeout, 0 );
					// TODO: add debug logging
				}

			} catch ( psocksxx::sockexception &e ) {

				// failed to accept client connection
				logger.error( std::string( "[worker] failed to accept connection: " ).append( e.what() ) );

				// update request count
				max_requests--;
				continue;

			}

			// handle client request(s)
			max_requests = serve_client( client_sock, max_requests );

			// destroy / close connection
			delete client_sock;

		};

	}


	void Worker::load_req_handlers() throw() {

		OptionsRequestHandler * options_handler;

		// OPTIONS handler
		options_handler = new OptionsRequestHandler();
		_req_handlers["OPTIONS"] = options_handler;

		/* request handlers */

		// REQMOD
		_req_handlers["REQMOD"]  = new ReqmodRequestHandler();
		options_handler->register_handler( _req_handlers["REQMOD"] );

		// RESPMOD
		_req_handlers["RESPMOD"] = new RespmodRequestHandler();
		options_handler->register_handler( _req_handlers["RESPMOD"] );

	}


	unsigned int Worker::serve_client( psocksxx::nsockstream * client_sock, unsigned int max_requests ) throw() {

		// logger
		Logger &logger = Logger::instance();

		icap::RequestHeader * req_header;
		icap::Response * response;
		RequestHandler * req_handler;


		// request header
		req_header  = icap::util::read_req_header( client_sock );
		logger.debug( std::string( "[worker] request header:\r\n" ).append( req_header->raw_data() ) );

		// check timeout
		if ( client_sock->timedout() ) {
			logger.warn( "[worker] communication timed out..." );
			return --max_requests;
		}

		// try to find a handler for the request
		req_handler = util::find_req_handler( _req_handlers, req_header->method() );

		// sanity check
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

		return --max_requests;

	}

} /* end of namespace bitz */


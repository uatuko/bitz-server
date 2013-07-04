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
#include "config.h"
#include "logger.h"
#include "util.h"

#include <icap/util.h>


namespace bitz {

	ReqmodRequestHandler::ReqmodRequestHandler() : RequestHandler( "REQMOD" ) {

		// initialise defaults
		_handlers_count = 0;
		_handlers       = NULL;

		// load modifier modules
		load_modules();

	}


	ReqmodRequestHandler::~ReqmodRequestHandler() {

		// cleanup modifier modules
		cleanup_modules();

		if ( _handlers != NULL ) {
			delete [] _handlers;
		}

	}


	icap::Response * ReqmodRequestHandler::process( icap::RequestHeader * req_header, socketlibrary::TCPSocket * socket ) throw() {

		icap::Request  * request;
		icap::Response * response = NULL;
		Modifier       * modifier;

		int i = 0;

		// logger
		Logger &logger = Logger::instance();


		// request
		request = new icap::Request( req_header );

		// read request data
		if (! icap::util::read_req_data( request, socket ) ) {

			logger.warn( "[reqmod] failed to read request data" );
			response = new icap::Response( icap::ResponseHeader::SERVER_ERROR );

		} else {

			logger.debug( std::string( "[reqmod] payload.req-hdr:\r\n").append( request->payload().req_header ) );
			logger.debug( std::string( "[reqmod] payload.req-body:\r\n").append( request->payload().req_body ) );
			logger.debug( std::string( "[reqmod] payload.res-hdr:\r\n").append( request->payload().res_header ) );
			logger.debug( std::string( "[reqmod] payload.res-body:\r\n").append( request->payload().res_body ) );

			/*
			*  loop through loaded modifier modules and grab responses
			*
			*  we will only send out the response from the last module
			*  unless a icap::ResponseHeader::OK is received
			*/
			for ( i = 0 ; i < _handlers_count; i++ ) {

				// sanity check
				if ( _handlers[i].name == "" ) {
					logger.info( "[reqmod] modifier not loaded, not trying to get a response" );
					continue;
				}

				// grab the response from modifier
				logger.debug( std::string( "[reqmod] getting response from modifier: " ).append( _handlers[i].name ) );
				modifier = _handlers[i].symbols.create();


				// check whether this is a preview request
				if ( request->preview_size() >= 0 ) {

					logger.debug( std::string( "[reqmod] message preview request, preview: " ).append( util::itoa( request->preview_size() ) ) );

					/* TODO: preview
					*  notes:
					*    + check for preview
					*        - if preview, user the preview() from the module to get the response
					*        - if the response is "100 continue" send it back to the client here and
					*          then read and append to the request obj and use modify() to get the response
					*/

				} else {

					logger.debug( "[reqmod] modify request" );
					response = modifier->modify( request );

				}


				// status 200 OK means content modified
				if ( response->header()->status() == icap::ResponseHeader::OK ) {
					logger.debug( "[reqmod] OK response received, not getting responses from other modifiers" );
					break;
				}

				// cleanup
				logger.debug( std::string( "[reqmod] cleaning up modifier: " ).append( _handlers[i].name ) );
				_handlers[i].symbols.destroy( modifier );

			}

		}

		// cleanup
		delete request;

		// sanity check
		if ( response == NULL ) {
			logger.warn( "[reqmod] no valid response from modifiers, creating a server error (500) response" );
			response = new icap::Response( icap::ResponseHeader::SERVER_ERROR );
		}

		return response;

	}


	void ReqmodRequestHandler::load_modules() throw() {

		int i = 0;
		int j = 0;
		const bitz::config_t &config = Config::instance().configs();

		// search for request handlers
		for ( i = 0; i < config.req_handlers_count; i++ ) {

			// we are only interested in REQMOD handlers
			if ( config.req_handlers[i].name == "REQMOD" ) {

				if ( config.req_handlers[i].modules_count > 0 ) {

					_handlers_count = config.req_handlers[i].modules_count;
					_handlers       = new handler_t[_handlers_count];

					// search for request handler modules
					for (j = 0; j < _handlers_count; j++ ) {

						// load module
						if ( load_modifier( config.req_handlers[i].modules[j].module, _handlers[j].symbols ) ) {
							_handlers[j].name = config.req_handlers[i].modules[j].name;
						} else {
							_handlers[j].name = "";
							// FIXME: error handling
						}

					}

				}

				// not interested in duplicate config entries
				break;
			}

		}


	}


	void ReqmodRequestHandler::cleanup_modules() throw() {

		int i = 0;

		// logger
		Logger &logger = Logger::instance();

		for ( i = 0; i < _handlers_count; i++ ) {

			logger.debug( std::string( "[reqmod] unloading module: " ).append( _handlers[i].name ) );

			// unload
			unload_modifier( _handlers[i].symbols.modifier );

		}

	}

} /* end of namespace bitz */


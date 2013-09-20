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

#include "request_handler.h"
#include "config.h"
#include "logger.h"
#include "util.h"

#include <dlfcn.h>
#include <icap/util.h>


namespace bitz {

	RequestHandler::RequestHandler( const std::string &method ) {

		// initialise defaults
		_handlers_count = 0;
		_handlers       = NULL;

		// update variables
		_req_handler.method = method;

		// load modifier modules
		load_modules();

	}


	RequestHandler::~RequestHandler() {

		// cleanup modifier modules
		cleanup_modules();

		if ( _handlers != NULL ) {
			delete [] _handlers;
		}

		Logger &logger = Logger::instance();
		logger.debug( std::string( "[req] exiting request handler [" ).append( _req_handler.method ).append( "]" ) );

	}


	const std::string &RequestHandler::method() const throw() {
		return _req_handler.method;
	}


	icap::Response * RequestHandler::process( icap::RequestHeader * req_header, psocksxx::iosockstream * socket ) throw() {


		icap::Request  * request;
		icap::Response * response = NULL;

		// logger
		Logger &logger = Logger::instance();


		// request
		request = new icap::Request( req_header );

		// read request data
		if (! icap::util::read_req_data( request, socket ) ) {

			logger.warn( "[req] failed to read request data" );
			response = new icap::Response( icap::ResponseHeader::SERVER_ERROR );

		} else {

			logger.debug( std::string( "[req] payload.req-hdr:\r\n").append( request->payload().req_header ) );
			logger.debug( std::string( "[req] payload.req-body:\r\n").append( request->payload().req_body ) );
			logger.debug( std::string( "[req] payload.res-hdr:\r\n").append( request->payload().res_header ) );
			logger.debug( std::string( "[req] payload.res-body:\r\n").append( request->payload().res_body ) );


			// check for message preview
			if ( request->preview_size() >= 0 ) {

				// process preview
				logger.debug( std::string( "[req] message preview request, preview: " ).append( util::itoa( request->preview_size() ) ) );
				response = process_preview( request, socket );

			}

			/*
			*  When we get here, if the response is NULL then either this is a
			*  pure REQMOD request without message preview or the preview was
			*  inconclusive (i.e. 100 Continue) and we have requested for the full
			*  request.
			*/
			if ( response == NULL ) {

				// process modify
				logger.debug( "[req] modify request" );
				response = process_modify( request );

			}

		}

		// cleanup
		delete request;

		// sanity check
		if ( response == NULL ) {
			logger.warn( "[req] no valid response from modifiers, creating a server error (500) response" );
			response = new icap::Response( icap::ResponseHeader::SERVER_ERROR );
		}

		return response;

	}


	bool RequestHandler::load_modifier( const std::string &file, Modifier::symbols_t &symbols ) throw() {

		// logger
		Logger &logger = Logger::instance();

		// vars
		const char* dlsym_error;

		// load the modifier module
		logger.debug( "[req] loading modifier: " + file );
		symbols.modifier = dlopen( file.c_str(), RTLD_LAZY | RTLD_LOCAL );

		if (! symbols.modifier ) {
			logger.warn( std::string( "[req] failed to load modifier: " ).append( file ).append( dlerror() ) );
			return false;
		}

		// reset errors
		dlerror();

		// load the symbols
		symbols.create = ( Modifier::create_t * ) dlsym( symbols.modifier, "create" );
		dlsym_error    = dlerror();

		if ( dlsym_error ) {
			logger.warn( std::string( "[req] failed to load create symbol: " ).append( dlsym_error ) );
			return false;
		}

		symbols.destroy = ( Modifier::destroy_t * ) dlsym( symbols.modifier, "destroy" );
		dlsym_error     = dlerror();

		if ( dlsym_error ) {
			logger.warn( std::string( "[req] failed to load destroy symbol: " ).append( dlsym_error ) );
			return false;
		}

		return true;

	}


	void RequestHandler::unload_modifier( void * modifier ) throw() {
		// unload the modifier module
		dlclose( modifier );
	}


	void RequestHandler::load_modules() throw() {

		int i = 0;
		int j = 0;
		const bitz::config_t &config = Config::instance().configs();

		// search for request handlers
		for ( i = 0; i < config.req_handlers_count; i++ ) {

			// we are only interested in handlers for the current method (e.g. REQMOD, RESPMOD)
			if ( config.req_handlers[i].name == method() ) {

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


	void RequestHandler::cleanup_modules() throw() {

		int i = 0;

		// logger
		Logger &logger = Logger::instance();

		for ( i = 0; i < _handlers_count; i++ ) {

			logger.debug( std::string( "[req] unloading module: " ).append( _handlers[i].name ) );

			// unload
			unload_modifier( _handlers[i].symbols.modifier );

		}

	}


	icap::Response * RequestHandler::process_preview( icap::Request * request, psocksxx::iosockstream * socket ) throw() {

		icap::Response * response = NULL;
		Modifier       * modifier;

		int i = 0;
		bool continue_status = false;

		// logger
		Logger &logger = Logger::instance();


		/*
		*  Loop through loaded modifier modules and grab responses
		*
		*  We will only get a chance to get a response from the first
		*  module. But if the first module returns a '100 Continue' response
		*  then we read the rest of the request here before returning a NULL
		*  response.
		*/
		for ( i = 0 ; i < _handlers_count; i++ ) {

			// sanity check
			if ( _handlers[i].name == "" ) {
				logger.info( "[req] modifier not loaded, not trying to get a response" );
				continue;
			}

			// grab the response from modifier
			logger.debug( std::string( "[req] getting preview response from modifier: " ).append( _handlers[i].name ) );
			modifier = _handlers[i].symbols.create();
			response = modifier->preview( request );

			// cleanup
			logger.debug( std::string( "[req] cleaning up modifier: " ).append( _handlers[i].name ) );
			_handlers[i].symbols.destroy( modifier );

			// check response status
			if ( ( response->header()->status() == icap::ResponseHeader::NO_CONTENT )
					|| ( response->header()->status() == icap::ResponseHeader::OK ) ) {
				// no further action needed, break out of the loop
				break;
			}

			if ( response->header()->status() == icap::ResponseHeader::CONTINUE ) {

				// read the full response
				continue_status = preview_continue( response, request, socket );

				// cleanup
				delete response;

				// sanity check
				if ( continue_status ) {

					// success - set the response to NULL
					response = NULL;

				} else {

					// something went wrong, server error
					response = new icap::Response( icap::ResponseHeader::SERVER_ERROR );

				}

				// exit the loop
				break;

			}

			// we shouldn't have got this far
			logger.info( std::string( "[req] unrecognised preview response from modifier: " ).append( _handlers[i].name ) );

		}

		return response;

	}


	icap::Response * RequestHandler::process_modify( icap::Request * request ) throw() {

		icap::Response * response = NULL;
		Modifier       * modifier;

		int i = 0;

		// logger
		Logger &logger = Logger::instance();


		/*
		*  Loop through loaded modifier modules and grab responses
		*
		*  We will only return the response from the last module
		*  unless a icap::ResponseHeader::OK is received
		*/
		for ( i = 0 ; i < _handlers_count; i++ ) {

			// sanity check
			if ( _handlers[i].name == "" ) {
				logger.info( "[req] modifier not loaded, not trying to get a response" );
				continue;
			}

			// grab the response from modifier
			logger.debug( std::string( "[req] getting modify response from modifier: " ).append( _handlers[i].name ) );
			modifier = _handlers[i].symbols.create();
			response = modifier->modify( request );

			// cleanup
			logger.debug( std::string( "[req] cleaning up modifier: " ).append( _handlers[i].name ) );
			_handlers[i].symbols.destroy( modifier );

			// status 200 OK means content modified
			if ( response->header()->status() == icap::ResponseHeader::OK ) {
				logger.debug( "[req] OK response received, not getting responses from other modifiers" );
				break;
			}

		}

		return response;

	}


	bool RequestHandler::preview_continue( icap::Response * response, icap::Request * request, psocksxx::iosockstream * socket ) throw() {

		bool status = false;

		// logger
		Logger &logger = Logger::instance();


		// sanity check
		if ( request->payload().ieof ) {

			// we can process a '100 Continue' only if an 'ieof' is not received
			logger.warn( "[req] illegal '100 Continue' response" );

		} else {

			/* read the full request */

			// send back the response first
			if ( icap::util::send_response( response, socket ) ) {

				// read the rest of the request
				status = icap::util::read_req_continue_data( request, socket );

			}

		}

		return status;

	}

} /* end of namespace bitz */


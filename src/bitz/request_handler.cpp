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

#include <dlfcn.h>


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

} /* end of namespace bitz */


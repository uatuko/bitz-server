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
#include "logger.h"

#include <dlfcn.h>


namespace bitz {

	RequestHandler::RequestHandler( const std::string &method ) {
		_req_handler.method = method;
	}


	RequestHandler::~RequestHandler() {
		Logger &logger = Logger::instance();
		logger.debug( std::string( "exiting request handler [" ).append( _req_handler.method ).append( "]" ) );
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
		logger.debug( "loading modifier: " + file );
		symbols.modifier = dlopen( file.c_str(), RTLD_LAZY );

		if (! symbols.modifier ) {
			logger.warn( std::string( "failed to load modifier: " ).append( file ).append( dlerror() ) );
			return false;
		}

		// reset errors
		dlerror();

		// load the symbols
		symbols.create = ( Modifier::create_t * ) dlsym( symbols.modifier, "create" );
		dlsym_error    = dlerror();

		if ( dlsym_error ) {
			logger.warn( std::string( "failed to load create symbol: " ).append( dlsym_error ) );
			return false;
		}

		symbols.destroy = ( Modifier::destroy_t * ) dlsym( symbols.modifier, "destroy" );
		dlsym_error     = dlerror();

		if ( dlsym_error ) {
			logger.warn( std::string( "failed to load destroy symbol: " ).append( dlsym_error ) );
			return false;
		}

		return true;

	}


	void RequestHandler::unload_modifier( void * modifier ) throw() {
		// unload the modifier module
		dlclose( modifier );
	}

} /* end of namespace bitz */


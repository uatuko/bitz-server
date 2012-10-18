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

#include <iostream>
#include <string>
#include <cstdlib>

#include "config.h"
#include "bitz/config.h"
#include "bitz/manager.h"
#include "bitz/logger.h"

using namespace bitz;

int main() {

	// initialise configurations
	Config &server_config = Config::instance();
	const config_t &config = server_config.initialise();

	// initialise the logger
	Logger &logger = Logger::instance( config.log_file, config.log_category );
	logger.info( std::string( PACKAGE_STRING ) + " initialised" );

	// start-up the manager
	Manager * manager = new Manager( config.port );

	// clean-up
	delete manager;

	return( EXIT_SUCCESS );

}


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
#include <libconfig.h++>

#include "config.h"
#include "bitz/manager.h"
#include "bitz/logger.h"

#ifndef BITZ_SERVER_CONFIG_FILE
#define BITZ_SERVER_CONFIG_FILE "/etc/bitz/bitz-server.conf"
#endif

using namespace bitz;

int main() {

	// main configurations
	libconfig::Config config;
	std::string log_file, log_category;

	try {
		config.readFile( BITZ_SERVER_CONFIG_FILE );
	} catch( const libconfig::FileIOException &e ) {
		std::cerr << "Failed to read config file: " << BITZ_SERVER_CONFIG_FILE << std::endl;
		return( EXIT_FAILURE );
	}

	try {
		config.lookupValue( "log_file", log_file );
		config.lookupValue( "log_category", log_category );
	} catch( const libconfig::SettingNotFoundException &e ) {
		std::cerr << "Missing configurations, \
			check 'log_file' and 'log_category' entries" << std::endl;
		return( EXIT_FAILURE );
	}

	// initialise the logger
	Logger &logger = Logger::instance( log_file, log_category );
	logger.log( 600, std::string( PACKAGE_STRING ) + " initialised" );

	// other configurations
	int port;
	try {
		config.lookupValue( "port", port );
	} catch( const libconfig::SettingNotFoundException &e ) {
		logger.log( 200, "port confguration not found, defaults to 4040" );
		port = 4040;
	}

	// TODO
	Manager * manager = new Manager( 4040 );

	// clean-up
	delete manager;

	return( EXIT_SUCCESS );
}


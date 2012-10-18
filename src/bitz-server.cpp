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
#include <libconfig.h++>

#include "config.h"
#include "bitz/manager.h"
#include "bitz/logger.h"

#define CONFIG_FILE "/etc/bitz/bitz-server.conf"

using namespace bitz;

int main() {

	// configurations
	libconfig::Config config;


	// initialise the logger
	Logger &logger = Logger::instance( "/tmp/bitz-server.log", "bitz-server" );
	logger.log( 600, std::string( PACKAGE_STRING ) + " initialised" );

	// TODO
	Manager * manager = new Manager( 4040 );

	// clean-up
	delete manager;

	return 0;
}


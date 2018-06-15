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

#include <cstdlib>

#include <config.h>
#include "bitz-server.h"
#include "bitz/config.h"
#include "bitz/util.h"
#include "spdlog/spdlog.h"


int main( int argc, char **argv ) {

	// initialise the server
	bitz::server::init();

	// read command line options
	bitz::server::options_t opt = bitz::server::read_options( argc, argv );

	// logger
	auto logger = spdlog::stdout_color_st( "bitz-server" );
	if ( opt.debug_flag == 1 ) {
		spdlog::set_level( spdlog::level::debug );
	}

	// initialise configurations
	bitz::Config &server_config = bitz::Config::instance();

	if ( opt.config_file != "" ) {
		server_config.initialise( opt.config_file );
	} else {
		server_config.initialise();
	}

	// get a copy of the configs
	const bitz::config_t &config = server_config.configs();

	// create directories
	if ( ( opt.debug_flag != 1 ) && (! bitz::util::mkdirp( bitz::util::dirpath( config.pid_file ) ) ) ) {
		logger->critical( "[core] failed to create run dir" );
		exit( EXIT_FAILURE );
	}

	if (! bitz::util::mkdirp( bitz::util::dirpath( config.log_file ) ) ) {
		logger->critical( "[core] failed to create log dir" );
		exit( EXIT_FAILURE );
	}


	// daemonize
	if ( opt.debug_flag != 1 ) {

		// close stdout logger
		spdlog::drop( "bitz-server" );

		// daemonize
		bitz::server::daemonize( bitz::util::dirpath( config.pid_file ).c_str(), config.pid_file.c_str() );

		// open a file logger
		logger = spdlog::rotating_logger_mt( "bitz-server", config.log_file, ( 1048576 * 1024 ), 7 );

	} else {
		logger->info( "{} initialised", PACKAGE_STRING );
	}


	// start the server
	bitz::server::start( config.port, config.max_workers, config.max_worker_requests, config.comm_timeout );

	// flush logs
	logger->flush();

	// run the server
	bitz::server::run();

	// we should never get here
	bitz::server::shutdown();

	return( EXIT_SUCCESS );

}


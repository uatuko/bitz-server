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
#include "bitz/logger.h"


int main( int argc, char **argv ) {

	// initialise the server
	bitz::server::init();

	// read command line options
	bitz::server::options_t opt = bitz::server::read_options( argc, argv );

	/*
	 * TODO: notes
	 *   + read options and act accordingly
	 *   + deamonize if needed
	 *   + start( port, children, requests )
	 *   + run() <- loop
	 *   + end
	 */

	// daemonize
	if ( opt.debug_flag != 1 ) {
		bitz::server::daemonize( "/tmp", "/tmp/root/var/run.pid" );
	}

	// initialise configurations
	bitz::Config &server_config = bitz::Config::instance();
	const bitz::config_t &config = server_config.initialise();

	// initialise the logger
	bitz::Logger &logger = bitz::Logger::instance( config.log_file, config.log_category );
	logger.info( std::string( PACKAGE_STRING ) + " initialised" );

	// manager
//	Manager * manager;
//
//	try {
//		// start-up the manager
//		manager = MANAGER = new Manager( config.port );
//
//		// spawn workers
//		manager->spawn( 1, 2 );
//	} catch( ManagerException &mex ) {
//		std::cout << mex.what() << std::endl;
//		return ( EXIT_FAILURE );
//	}
//
//
//	/* loop until termination signal arrives */
//	sigset_t mask, oldmask;
//	sigemptyset( &mask );
//	sigaddset( &mask, SIGTERM );
//	sigaddset( &mask, SIGQUIT );
//	sigaddset( &mask, SIGINT );
//
//	sigprocmask ( SIG_BLOCK, &mask, &oldmask );
//
//	while (! termination_in_progress ) {
//		std::cout << "[" << getpid() << "] inside termination loop" << std::endl;
//		sigsuspend (&oldmask);
//		sigprocmask (SIG_UNBLOCK, &mask, NULL);
//		manager->manager_workers();
//		sigprocmask (SIG_BLOCK, &mask, &oldmask);
//	}

	std::cout << "no mans land" << std::endl;
	// clean-up (in theory we shouldn't get here)
//	delete manager;

	return( EXIT_SUCCESS );

}


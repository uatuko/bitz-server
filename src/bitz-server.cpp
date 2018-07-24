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
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <syslog.h>

#include <spdlog/spdlog.h>

#include <config.h>
#include "bitz-server.h"

namespace bitz {

	namespace server {

		static server_t globals;

		void init() {

			// initialise defaults
			globals.pid         = -1;
			globals.pidfd       = -1;
			globals.terminating = 0;
			globals.daemon      = false;

			// block signals
			sigset_t ignore_mask;

			// set signal mask - signals we want to block
			sigemptyset(&ignore_mask);
			sigaddset(&ignore_mask, SIGTSTP);				// ignore Tty stop signals
			sigaddset(&ignore_mask, SIGTTOU);				// ignore Tty background writes
			sigaddset(&ignore_mask, SIGTTIN);				// ignore Tty background reads
			sigprocmask(SIG_BLOCK, &ignore_mask, NULL);		// block the above specified signals

		}


		void daemonize( const char *rundir, const char *pidfile ) {

			pid_t pid;
			long i;
			char str[10];

			/* fork daemon */
			pid = fork();
			if ( pid < 0 ) {
				exit( EXIT_FAILURE );
			}

			// exit the parent
			if ( pid > 0 ) {
				exit( EXIT_SUCCESS );
			}

			// get a new process group
			if ( setsid() < 0 ) {
				exit(EXIT_FAILURE);
			}


			// 2nd fork (to make PID != SID)
			pid = fork();
			if ( pid < 0 ) {
				exit( EXIT_FAILURE );
			}

			// exit the parent
			if ( pid > 0 ) {
				exit( EXIT_SUCCESS );
			}


			/* child (a.k.a daemon) continues */
			// set file permissions (750)
			umask( 027 );

			// change running directory
			chdir( rundir );

			// close all open file descriptors
			for ( int fd = sysconf( _SC_OPEN_MAX ); fd > 0; fd-- ) {
				close( fd );
			}


			// logger (syslog)
			setlogmask( LOG_UPTO( LOG_INFO ) );
			openlog( PACKAGE_NAME, LOG_CONS, LOG_USER );

			// notify
			syslog( LOG_NOTICE, "starting daemon (version %s)", PACKAGE_VERSION );


			/* lock pid file to ensure we have only one copy */

			globals.pidfd = open( pidfile, O_RDWR | O_CREAT, 0600 );
			if ( globals.pidfd == -1 ) {
				syslog( LOG_ERR, "could not open pid lock file: %s", pidfile );
				exit( EXIT_FAILURE );
			}

			if ( lockf( globals.pidfd, F_TLOCK, 0 ) == -1 ) {
				syslog( LOG_ERR, "could not lock pid lock file: %s", pidfile);
				exit( EXIT_FAILURE );
			}

			// get and format pid
			globals.pid = getpid();
			sprintf( str, "%d\n", globals.pid );

			// write pid to lockfile
			write( globals.pidfd, str, strlen( str ) );

			// update status
			globals.daemon = true;

		}


		void shutdown() {

			// notify
			if ( globals.daemon && ( getpid() == globals.pid ) ) {
				syslog( LOG_NOTICE, "shutting down daemon (version %s)", PACKAGE_VERSION );
			}

			// close pid file
			if ( globals.pidfd != -1 ) {
				close( globals.pidfd );
			}

			// flush logs
			spdlog::get( "bitz-server" )->flush();

			// close logger (syslog)
			closelog();

		}


		options_t read_options( int argc, char **argv ) {

			int optidx, optchar;
			options_t options;

			options.config_file = "";

			struct option lopts[] = {
				{ "config", required_argument, 0, 'c' },
				{ "debug", no_argument, &options.debug_flag, 1 },
				{ "help", no_argument, 0, 'h' },
				{ "usage", no_argument, 0, 'h' },
				{ "version", no_argument, 0, 'v' },
				{ 0, 0, 0, 0 }
			};

			while ( true ) {

				optidx  = 0;
				optchar = getopt_long( argc, argv, "c:hv", lopts, &optidx );

				// sanity check
				if ( optchar == -1 ) {
					break;
				}

				switch ( optchar ) {
					case 0:
						// TODO:
						break;

					case 'c':
						options.config_file = optarg;
						break;

					case 'h':
						print_usage();
						exit( EXIT_SUCCESS );
						break;

					case 'v':
						print_version();
						exit( EXIT_SUCCESS );

					case '?':
						print_usage();
						exit( EXIT_FAILURE );
						break;

					default:
						// TODO:
						break;
				}

			}

			return options;

		}


		void print_version() {

			std::cout << PACKAGE_STRING << std::endl;
			std::cout << "" << std::endl;
			std::cout << "Copyright (C) 2012-2013 Uditha Atukorala" << std::endl;
			std::cout << "" << std::endl;
			std::cout << "This program is free software; you can redistribute it and/or modify" << std::endl;
			std::cout << "it under the terms of the GNU General Public License as published by" << std::endl;
			std::cout << "the Free Software Foundation; either version 3 of the License, or" << std::endl;
			std::cout << "(at your option) any later version." << std::endl;
			std::cout << "" << std::endl;
			std::cout << "This program is distributed in the hope that it will be useful," << std::endl;
			std::cout << "but WITHOUT ANY WARRANTY; without even the implied warranty of" << std::endl;
			std::cout << "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the" << std::endl;
			std::cout << "GNU General Public License (http://gnu.org/licenses/gpl.html)" << std::endl;
			std::cout << "for more details." << std::endl;
			std::cout << "" << std::endl;

		}


		void print_usage() {

			std::cout << "usage: " << PACKAGE_NAME << " "
				<< "[--version] [--help] [--usage] [--debug] [--config=<config file>]"
				<< std::endl;

			std::cout << "" << std::endl;
			std::cout << "See the man pages for more information" << std::endl;

		}


		void start( int port, unsigned int children, int max_requests, int comm_timeout ) {

			globals.evloop = std::make_unique<bitz::EvLoop>();
			globals.evloop->start( port );

		}


		void run() {

			// sanity check
			if ( !globals.evloop ) {
				return;
			}

			globals.evloop->run();

		}

	} /* end of namespace server */

} /* end of namespace bitz */




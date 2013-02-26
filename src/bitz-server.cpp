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
#include <csignal>
#include <fcntl.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <config.h>
#include "bitz-server.h"

namespace bitz {

	namespace server {

		static server_t globals;

		void init() {

			// initialise defaults
			globals.pid_handle  = -1;
			globals.manager     = NULL;
			globals.terminating = 0;

			// signal handlers
			init_signal_handlers();

		}


		void init_signal_handlers() {

			sigset_t ignore_mask;

			// set signal mask - signals we want to block
			sigemptyset(&ignore_mask);
			sigaddset(&ignore_mask, SIGTSTP);				// ignore Tty stop signals
			sigaddset(&ignore_mask, SIGTTOU);				// ignore Tty background writes
			sigaddset(&ignore_mask, SIGTTIN);				// ignore Tty background reads
			sigprocmask(SIG_BLOCK, &ignore_mask, NULL);		// block the above specified signals

			init_sigchld_handler();
			init_sigterm_handler();
			init_sigquit_handler();
			init_sigint_handler();

		}


		void init_sigchld_handler() {

			struct sigaction sa;

			// block all other signals
			sigfillset( &sa.sa_mask );

			// signal handler proc
			sa.sa_sigaction = &sigchld_handler;

			// flags
			sa.sa_flags = SA_SIGINFO;

			if ( sigaction( SIGCHLD, &sa, NULL ) < 0 ) {
				exit( EXIT_FAILURE );
			}

		}


		void sigchld_handler( int sig, siginfo_t *siginfo, void *context ) {

			pid_t worker_pid;
			int status;

			std::cout << "[" << getpid() << "] inside zombie deleter: ";
			while ( ( worker_pid = waitpid( WAIT_ANY, &status, WNOHANG ) ) > 0 ) {
				printf( "child %ld terminated with status %d\n", worker_pid, status );

				if ( globals.manager != NULL ) {
					globals.manager->reap_worker( worker_pid );
				}
			}

		}


		void init_sigterm_handler() {

			struct sigaction sa;

			// block all other signals
			sigfillset( &sa.sa_mask );

			// signal handler proc
			sa.sa_sigaction = &sigterm_handler;

			// flags
			sa.sa_flags = SA_SIGINFO;

			if ( sigaction( SIGTERM, &sa, NULL ) < 0 ) {
				exit( EXIT_FAILURE );
			}

		}


		void sigterm_handler( int sig, siginfo_t *siginfo, void *context ) {

			std::cout << "[" << getpid() << "] inside SIGTERM handler" << std::endl;
			termination_handler( sig, siginfo, context );

		}


		void init_sigquit_handler() {

			struct sigaction sa;

			// block all other signals
			sigfillset( &sa.sa_mask );

			// signal handler proc
			sa.sa_sigaction = &sigquit_handler;

			// flags
			sa.sa_flags = SA_SIGINFO;

			if ( sigaction( SIGQUIT, &sa, NULL ) < 0 ) {
				exit( EXIT_FAILURE );
			}

		}


		void sigquit_handler( int sig, siginfo_t *siginfo, void *context ) {

			std::cout << "[" << getpid() << "] inside SIGQUIT handler" << std::endl;
			termination_handler( sig, siginfo, context );

		}


		void init_sigint_handler() {

			struct sigaction sa;

			// block all other signals
			sigfillset( &sa.sa_mask );

			// signal handler proc
			sa.sa_sigaction = &sigint_handler;

			// flags
			sa.sa_flags = SA_SIGINFO;

			if ( sigaction( SIGINT, &sa, NULL ) < 0 ) {
				exit( EXIT_FAILURE );
			}

		}


		void sigint_handler(  int sig, siginfo_t *siginfo, void *context ) {

			std::cout << "[" << getpid() << "] inside SIGQINT handler" << std::endl;
			termination_handler( sig, siginfo, context );

		}


		void daemonize( char *rundir, char *pidfile ) {

			pid_t pid, sid;
			long i;
			char str[10];

			// logger (syslog)
			setlogmask( LOG_UPTO( LOG_INFO ) );
			openlog( PACKAGE_STRING, LOG_CONS, LOG_USER );
			syslog( LOG_INFO, "starting daemon (version %s)", PACKAGE_VERSION );

			// check parent process id value
			if ( getppid() == 1 ) {
				// we are already a daemon
				return;
			}

			/* fork daemon */
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

			// get a new process group
			sid = setsid();
			if ( sid < 0 ) {
				exit(EXIT_FAILURE);
			}

			// route I/O connections
			close( STDIN_FILENO );
			close( STDOUT_FILENO );
			close( STDERR_FILENO );

			// change running directory
			chdir( rundir );


			/* lock pid file to ensure we have only one copy */

			globals.pid_handle = open( pidfile, O_RDWR | O_CREAT, 0600 );
			if ( globals.pid_handle == -1 ) {
				syslog( LOG_ERR, "could not open pid lock file: %s", pidfile );
				exit( EXIT_FAILURE );
			}

			if ( lockf( globals.pid_handle, F_TLOCK, 0 ) == -1 ) {
				syslog( LOG_ERR, "could not lock pid lock file: %s", pidfile);
				exit( EXIT_FAILURE );
			}

			// get and format pid
			sprintf( str, "%d\n", getpid() );

			// write pid to lockfile
			write( globals.pid_handle, str, strlen( str ) );

			// cleanup
			closelog();

		}


		void shutdown_daemon() {
			close( globals.pid_handle );
		}


		void termination_handler( int sig, siginfo_t *siginfo, void *context ) {

			std::cout << "[" << getpid() << "] inside termination handler" << std::endl;

			// exit by re-raising the signal if termination
			// already in progress
			if ( globals.terminating ) {
				std::cout << "[" << getpid() << "] already terminating" << std::endl;
				raise( sig );
			}

			// update termination status
			globals.terminating = 1;


			if ( globals.manager != NULL ) {

				// shutdown the manager
				globals.manager->shutdown();

				// clean-up
				delete globals.manager;

			}

			// cleanup
			shutdown_daemon();

			// re-raise the signal after reactivating the signal's default action
			signal( sig, SIG_DFL );
			raise( sig );

		}

	} /* end of namespace server */

} /* end of namespace bitz */




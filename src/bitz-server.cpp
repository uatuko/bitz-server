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
#include <sys/wait.h>

#include "config.h"
#include "bitz/config.h"
#include "bitz/manager.h"
#include "bitz/logger.h"

using namespace bitz;

// forward declarations
void init_signal_handlers();
void init_sigchld_handler();
void init_sigterm_handler();
void init_sigquit_handler();
void init_sigint_handler();
void sigchld_handler( int, siginfo_t *, void * );
void sigterm_handler( int, siginfo_t *, void * );
void sigquit_handler( int, siginfo_t *, void * );
void sigint_handler( int, siginfo_t *, void * );

void termination_handler( int, siginfo_t *, void * );


// globals
Manager * MANAGER;

volatile sig_atomic_t termination_in_progress = 0;

int main() {

	// initialise configurations
	Config &server_config = Config::instance();
	const bitz::config_t &config = server_config.initialise();

	// initialise the logger
	Logger &logger = Logger::instance( config.log_file, config.log_category );
	logger.info( std::string( PACKAGE_STRING ) + " initialised" );

	// initialise signal handlers
	init_signal_handlers();

	// manager
	Manager * manager;

	try {
		// start-up the manager
		manager = MANAGER = new Manager( config.port );

		// spawn workers
		manager->spawn( 1, 2 );
	} catch( ManagerException &mex ) {
		std::cout << mex.what() << std::endl;
		return ( EXIT_FAILURE );
	}


	/* loop until termination signal arrives */
	sigset_t mask, oldmask;
	sigemptyset( &mask );
	sigaddset( &mask, SIGTERM );
	sigaddset( &mask, SIGQUIT );
	sigaddset( &mask, SIGINT );

	sigprocmask (SIG_BLOCK, &mask, &oldmask);

	while (! termination_in_progress ) {
		std::cout << "[" << getpid() << "] inside termination loop" << std::endl;
		sigsuspend (&oldmask);
		sigprocmask (SIG_UNBLOCK, &mask, NULL);
		manager->manager_workers();
		sigprocmask (SIG_BLOCK, &mask, &oldmask);
	}

	std::cout << "no mans land" << std::endl;
	// clean-up (in theory we shouldn't get here)
	delete manager;

	return( EXIT_SUCCESS );

}



/* signal handlers */
void init_signal_handlers() {
	init_sigchld_handler();
	init_sigterm_handler();
	init_sigquit_handler();
	init_sigint_handler();
}



/* SIGCHLD handler */
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

	Manager * manager = MANAGER;
	pid_t worker_pid;
	int status;

	std::cout << "[" << getpid() << "] inside zombie deleter: ";
	while ( ( worker_pid = waitpid( WAIT_ANY, &status, WNOHANG ) ) > 0 ) {
		printf( "child %ld terminated with status %d\n", worker_pid, status );
		manager->reap_worker( worker_pid );
	}

}



/* SIGTERM handler */
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



/* SIGQUIT handler */
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



/* SIGINT handler */
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



/* termination handler */
void termination_handler( int sig, siginfo_t *siginfo, void *context ) {

	std::cout << "[" << getpid() << "] inside termination handler" << std::endl;

	// exit by re-raising the signal if termination
	// already in progress
	if ( termination_in_progress ) {
		std::cout << "[" << getpid() << "] already terminating" << std::endl;
		raise( sig );
	}

	// update termination status
	termination_in_progress = 1;


	Manager * manager = MANAGER;

	// shutdown the manager
	manager->shutdown();

	// clean-up
	delete manager;

	// re-raise the signal after reactivating the signal's default action
	signal( sig, SIG_DFL );
	raise( sig );

}


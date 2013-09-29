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

#ifndef BITZ_SERVER_H
#define BITZ_SERVER_H

#include "bitz/manager.h"

#include <csignal>


namespace bitz {

	namespace server {

		/**
		*   Structure to hold server globals initialised
		*   by the init() method.
		*/
		struct server_t {
			int pid_handle;
			volatile sig_atomic_t terminating;
			bool daemon;

			bitz::Manager * manager;
		};

		/**
		*   Structure to hold command line options.
		*/
		struct options_t {
			int          debug_flag;
			std::string  config_file;
		};

		void init();
		void init_signal_handlers();
		void init_sigchld_handler();
		void init_sigterm_handler();
		void init_sigquit_handler();
		void init_sigint_handler();
		void sigchld_handler( int sig, siginfo_t * sig_info, void * context );
		void sigterm_handler( int sig, siginfo_t * sig_info, void * context );
		void sigquit_handler( int sig, siginfo_t * sig_info, void * context );
		void sigint_handler( int sig, siginfo_t * sig_info, void * context );

		void daemonize( const char * run_dir, const char * pid_file );
		void shutdown();
		void termination_handler( int sig, siginfo_t * sig_info, void * context );

		/**
		*   Read command line options and return a options_t structure.
		*   This method will terminate the program with an exit status
		*   of EXIT_FAILURE if any errors are found while reading options.
		*   If --version or --help options are found this will print the
		*   appropriate message to the standard output and terminate with
		*   EXIT_SUCCESS.
		*
		*   @param argc same as main() argc
		*   @param argv same as main() argv
		*   @return read options
		*/
		options_t read_options( int argc, char **argv );

		/**
		*   Print version information to the standard output
		*/
		void print_version();

		/**
		*   Print usage message to the standard output
		*/
		void print_usage();

		/**
		*   Start the server by creating a bitz::Manager instance and spawn
		*   workers.
		*
		*   @param port port number to listen to
		*   @param children number of children to spawn
		*   @param max_request maximum number of requests that a child will serve
		*   @param comm_timeout communication timeout value in seconds
		*/
		void start( int port, unsigned int children, int max_requests, int comm_timeout );

		/**
		*   Run the server managing workers until a termination signal is received.
		*   This process will never return, instead the termination signal will
		*   end the process.
		*/
		void run();

	} /* end of namespace server */

} /* end of namespace bitz */

#endif /* !BITZ_SERVER_H */


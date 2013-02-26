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


namespace bitz {

	namespace server {

		/**
		*   Structure to hold server globals initialised
		*   by the init() method.
		*/
		struct server_t {
			int pid_handle;
			volatile sig_atomic_t termination_in_progress;

			bitz::Manager * manager;
		} globals;

		void daemonize( char * run_dir, char * pid_file );
		void shutdown_daemon();
		void termination_handler( int signal, siginfo_t * sig_info, void * context );


	} /* end of namespace server */

} /* end of namespace bitz */

#endif /* !BITZ_SERVER_H */


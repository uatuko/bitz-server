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

#include "manager.h"
#include "logger.h"
#include "util.h"
#include "options_request_handler.h"
#include "reqmod_request_handler.h"

#include <cstdlib>
#include <sstream>
#include <csignal>

namespace bitz {

	Manager::Manager( unsigned short port, const std::string &address, int backlog ) throw( ManagerException ) {

		// initialise manager
		_manager.worker        = false;
		_manager.max_workers   = 0;
		_manager.workers_count = 0;
		_manager.worker_id     = 0;
		_manager.socket        = NULL;
		_manager.worker_pool   = NULL;

		// request handlers
		_req_handlers["OPTIONS"] = new OptionsRequestHandler();

		// FIXME: these should be able to dynamically loaded and configurable
		_req_handlers["REQMOD"]  = new ReqmodRequestHandler();

		// initialise listening socket
		try {
			if ( address.empty() ) {
				_manager.socket = new socketlibrary::TCPServerSocket( port, backlog );
			} else {
				_manager.socket = new socketlibrary::TCPServerSocket( address, port, backlog );
			}
		} catch ( socketlibrary::SocketException &sex ) {
			throw ManagerException( "failed to initialise socket" );
		}

		Logger &logger = Logger::instance();
		logger.debug( "manager initialised" );

	}


	Manager::~Manager() {

		Logger &logger = Logger::instance();

		if ( _manager.worker ) {
			logger.debug( "[worker] cleaning up manager" );
		} else {
			logger.debug( "shutting down manager" );
		}

		delete _req_handlers["OPTIONS"];

		delete [] _manager.worker_pool;
		delete _manager.socket;

	}


	void Manager::spawn( unsigned int max_workers, unsigned int max_worker_requests ) throw( ManagerException ) {

		_manager.max_workers         = max_workers;
		_manager.max_worker_requests = max_worker_requests;
		_manager.worker_pool         = new worker_pool_t[max_workers];

		// pre-fork workers
		if (! _manager.worker ) {
			for ( unsigned int i = 0; i < max_workers; i++ ) {

				try {
					spawn_worker( i );
				} catch ( ManagerException &mex ) {
					throw mex;
				}

			}
		}

	}


	void Manager::spawn_worker( unsigned int worker_id ) throw( ManagerException ) {

		Logger &logger = Logger::instance();
		pid_t worker_pid;

		// create a worker child
		worker_pid = fork();
		if ( worker_pid == -1 ) {
			throw ManagerException( "failed to create worker", true );
		}

		if ( worker_pid == 0 ) {

			/* worker */

			_manager.worker    = true;
			_manager.worker_id = worker_id;
			_manager.worker_pool[worker_id].worker     = new Worker();
			_manager.worker_pool[worker_id].worker_id  = worker_id;
			_manager.worker_pool[worker_id].worker_pid = worker_pid;

			_manager.worker_pool[worker_id].worker->run( _manager.socket, _req_handlers, _manager.max_worker_requests );
			logger.info( std::string( "end of cycle, worker[" ).append( util::itoa( worker_id ) ).append( "]" ) );

			delete _manager.worker_pool[worker_id].worker;
			_exit( EXIT_SUCCESS );

		} else {

			/* manager */

			_manager.workers_count++;

			_manager.worker = false;
			_manager.worker_pool[worker_id].worker     = NULL;
			_manager.worker_pool[worker_id].worker_id  = worker_id;
			_manager.worker_pool[worker_id].worker_pid = worker_pid;

		}

	}


	void Manager::shutdown( bool graceful ) throw() {

		// logger
		Logger &logger = Logger::instance();

		if ( _manager.worker ) {

			/* worker: cleanup */
			delete _manager.worker_pool[_manager.worker_id].worker;

		} else {

			/* manager: stop all child processes */

			for (unsigned int i = 0; i < _manager.max_workers; i++ ) {
				if ( _manager.worker_pool[i].worker_pid != 0 ) {
					if ( graceful ) {
						kill( _manager.worker_pool[i].worker_pid, SIGTERM );
						logger.debug( std::string( "manager: sending SIGTERM to worker[" ).append( util::itoa( i ) )
								.append( "], pid: " ).append( util::itoa( _manager.worker_pool[i].worker_pid ) ) );
					} else {
						kill( _manager.worker_pool[i].worker_pid, SIGKILL );
						logger.debug( std::string( "manager: sending SIGKILL to worker[" ).append( util::itoa( i ) )
								.append( "], pid: " ).append( util::itoa( _manager.worker_pool[i].worker_pid ) ) );
					}
				} else {
					logger.debug( std::string( "manager: worker[" ).append( util::itoa( i ) ).append( " already closed" ) );
				}
			}
		}

	}


	void Manager::reap_worker( pid_t worker_pid ) throw() {

		// logger
		Logger &logger = Logger::instance();
		logger.debug( std::string( "reaping worker, pid: " ).append( util::itoa( worker_pid ) ) );

		if (! _manager.worker ) {
			for (unsigned int i = 0; i < _manager.max_workers; i++ ) {
				if ( _manager.worker_pool[i].worker_pid == worker_pid ) {

					// reap the dead worker
					_manager.worker_pool[i].worker_pid = 0;
					_manager.workers_count--;

					// break out the loop
					break;

				}
			}
		}

	}


	void Manager::manager_workers() throw() {

		if (! _manager.worker ) {

			// check the worker count
			while ( _manager.workers_count != _manager.max_workers ) {

				// we are missing workers, find out who
				for (unsigned int i = 0; i < _manager.max_workers; i++ ) {
					if ( _manager.worker_pool[i].worker_pid == 0 ) {
						// spawn a worker for the missing
						spawn_worker( i );
					}
				}

			}

		}

	}

} /* end of namespace bitz */


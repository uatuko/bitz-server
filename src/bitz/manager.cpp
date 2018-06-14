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
#include "util.h"

#include <cstdlib>
#include <sstream>
#include <csignal>


namespace bitz {

	Manager::Manager( unsigned short port, const std::string &address, int backlog ) throw( ManagerException ) {

		// initialise manager
		_manager.worker              = false;
		_manager.max_workers         = 0;
		_manager.max_worker_requests = 0;
		_manager.comm_timeout        = 0;

		_manager.workers_count = 0;
		_manager.worker_id     = 0;
		_manager.socket        = NULL;
		_manager.worker_pool   = NULL;

		// initialise listening socket
		try {

			// network socket address
			psocksxx::nsockaddr naddr( address.c_str(), port );

			_manager.socket = new psocksxx::tcpnsockstream();
			_manager.socket->bind( &naddr, true );
			_manager.socket->listen( backlog );

		} catch ( psocksxx::sockexception &e ) {
			throw ManagerException( std::string( "failed to initialise socket, " ).append( e.what() ) );
		}

		// logger
		_logger = spdlog::get( "bitz-server" );
		_logger->debug( "manager initialised" );

	}


	Manager::~Manager() {

		if ( _manager.worker ) {
			_logger->debug( "[worker] cleaning up manager" );
		} else {
			_logger->debug( "[manager] shutting down manager" );
		}

		delete [] _manager.worker_pool;
		delete _manager.socket;

	}


	void Manager::spawn( unsigned int max_workers, unsigned int max_worker_requests, unsigned int comm_timeout ) throw( ManagerException ) {

		_manager.max_workers         = max_workers;
		_manager.max_worker_requests = max_worker_requests;
		_manager.comm_timeout        = comm_timeout;
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

			_manager.worker_pool[worker_id].worker->run( _manager.socket, _manager.max_worker_requests, _manager.comm_timeout );
			_logger->info( std::string( "end of cycle, worker[" ).append( util::itoa( worker_id ) ).append( "]" ) );

			delete _manager.worker_pool[worker_id].worker;
			_exit( EXIT_SUCCESS );

		} else {

			/* manager */
			_logger->info( std::string( "[manager] worker spawned with pid: " ).append( util::itoa( worker_pid) ) );

			_manager.workers_count++;

			_manager.worker = false;
			_manager.worker_pool[worker_id].worker     = NULL;
			_manager.worker_pool[worker_id].worker_id  = worker_id;
			_manager.worker_pool[worker_id].worker_pid = worker_pid;

		}

	}


	void Manager::shutdown( bool graceful ) throw() {

		if ( _manager.worker ) {

			_logger->info( "[worker] manager shutdown request received" );

			/* worker: cleanup */
			delete _manager.worker_pool[_manager.worker_id].worker;

		} else {

			/* manager: stop all child processes */
			_logger->info( "[manager] shutdown request received" );

			for (unsigned int i = 0; i < _manager.max_workers; i++ ) {
				if ( _manager.worker_pool[i].worker_pid != 0 ) {
					if ( graceful ) {
						kill( _manager.worker_pool[i].worker_pid, SIGTERM );
						_logger->debug( std::string( "[manager] sending SIGTERM to worker[" ).append( util::itoa( i ) )
								.append( "], pid: " ).append( util::itoa( _manager.worker_pool[i].worker_pid ) ) );
					} else {
						kill( _manager.worker_pool[i].worker_pid, SIGKILL );
						_logger->debug( std::string( "[manager] sending SIGKILL to worker[" ).append( util::itoa( i ) )
								.append( "], pid: " ).append( util::itoa( _manager.worker_pool[i].worker_pid ) ) );
					}
				} else {
					_logger->debug( std::string( "[manager] worker[" ).append( util::itoa( i ) ).append( "] already closed" ) );
				}
			}
		}

	}


	void Manager::reap_worker( pid_t worker_pid ) throw() {

		_logger->debug( std::string( "reaping worker, pid: " ).append( util::itoa( worker_pid ) ) );

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
						try {
							// spawn a worker for the missing
							spawn_worker( i );
						} catch ( ManagerException &mex ) {
							_logger->warn( std::string( "[manager] failed to spawn worker[" ).append( util::itoa( i ) ).append( "], exception: ").append( mex.what() ) );
						}
					}
				}

			}

		}

	}

} /* end of namespace bitz */


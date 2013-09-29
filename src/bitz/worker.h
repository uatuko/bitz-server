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

#ifndef BITZ_WORKER_H
#define BITZ_WORKER_H

#include <psocksxx/tcpnsockstream.h>

#include "common.h"


namespace bitz {

	class Worker {
	public:
		Worker();
		virtual ~Worker();

		virtual void run( psocksxx::tcpnsockstream * server_sock,
				unsigned int max_requests, unsigned int comm_timeout = 0 ) throw();

	private:
		req_handlers_t    _req_handlers;

		virtual void load_req_handlers() throw();
		virtual unsigned int serve_client( psocksxx::nsockstream * client_sock, unsigned int max_requests ) throw();

	};

} /* end of namespace bitz */

#endif /* !BITZ_WORKER_H */


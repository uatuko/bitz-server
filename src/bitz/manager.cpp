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

namespace bitz {

	Manager::Manager( unsigned short port, const std::string &address, int backlog ) {


		if ( address.empty() ) {
			this->socket = new socketlibrary::TCPServerSocket( port, backlog );
		} else {
			this->socket = new socketlibrary::TCPServerSocket( address, port, backlog );
		}

		Logger &logger = Logger::instance();
		logger.debug( "manager initialised" );

	}

	Manager::~Manager() {

		Logger &logger = Logger::instance();
		logger.debug( "shutting down manager" );

		delete this->socket;

	}

	void Manager::spawn() {}

} // end of namespace bitz


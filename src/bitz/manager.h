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

#ifndef BITZ_MANAGER_H
#define BITZ_MANAGER_H

#include <string>              // for string type
#include <socket/socket.h>     // socket-library

namespace bitz {

	class Manager {
		public:
			/**
			 * Note: backlog = SOMAXCONN (from sys/socket.h)
			 */
			Manager( unsigned short port, const std::string &address = "", int backlog = 128 );

			/**
			 * deconstructor
			 */
			~Manager();

			void spawn();

		private:
			socketlibrary::TCPServerSocket * socket;
	};

} // end of namespace bitz

#endif /* !BITZ_MANAGER_H */


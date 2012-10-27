/*
 *	C++ ICAP library
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

#include "util.h"

#include <string>
#include <vector>

namespace icap {

	namespace util {

		int read_line( socketlibrary::TCPSocket * socket, char * buf, int buf_length ) throw() {

			int i  = 0, n;
			char c = '\0';

			while ( i < ( buf_length - 1 ) ) {

				n = socket->recv( &c, 1 );

				if ( n > 0 ) {
					if ( c == '\r' ) {
						n = socket->peek( &c, 1 );
						if ( ( n > 0 ) && ( c == '\n' ) ) {
							socket->recv( &c, 1 );
							break;    // end of line
						}
					}

					buf[i] = c;
					i++;
				} else {
					break;    // nothing read from socket
				}

			}

			buf[i] = '\0';
			return i;

		}

		icap::RequestHeader * read_req_header( socketlibrary::TCPSocket * socket ) {

			char buffer[ICAP_BUFFER_LENGTH];
			int  n = 0;
			std::vector<std::string> data;

			while ( ( n = read_line( socket, buffer, ICAP_BUFFER_LENGTH ) ) > 0 ) {
				// FIXME: this is not correct if a line is longer than the buffer length
				data.push_back( buffer );
			}

			icap::RequestHeader * req_header = new icap::RequestHeader( data );
			return req_header;
		}

	} /* end of namespace util */

} /* end of namespace icap */


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
#include <algorithm>
#include <functional>
#include <locale>


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


		std::vector<std::string> split( const std::string &str, const std::string &delimiter ) throw() {

			std::vector<std::string> result;
			size_t current;
			size_t next = -1;

			do {
				current = next + 1;
				next    = str.find_first_of( delimiter, current );
				result.push_back( str.substr( current, ( next - current ) ) );
			} while ( next != std::string::npos );

			return result;

		}


		std::string &ltrim( std::string &str ) throw() {
			str.erase( str.begin(), std::find_if( str.begin(), str.end(), std::not1( std::ptr_fun<int, int>( std::isspace ) ) ) );
			return str;
		}


		std::string &rtrim( std::string &str ) throw() {
			str.erase( std::find_if( str.rbegin(), str.rend(), std::not1( std::ptr_fun<int, int>( std::isspace ) ) ).base(), str.end() );
			return str;
		}


		std::string &trim( std::string &str ) throw() {
			return ltrim( rtrim( str ) );
		}


		icap::RequestHeader * read_req_header( socketlibrary::TCPSocket * socket ) throw() {

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


		void send_response( icap::Response * response, socketlibrary::TCPSocket * socket ) throw() {

			icap::ResponseHeader * header;

			header = response->header();

			// TODO: send the response back
			std::string line = "ICAP/1.0 500 Server Error\n";
			socket->send( line.c_str(), line.length() );
		}


	} /* end of namespace util */

} /* end of namespace icap */


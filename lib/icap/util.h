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

#ifndef ICAP_UTIL_H
#define ICAP_UTIL_H

#include <sstream>

#include <socket/socket.h>
#include "request_header.h"
#include "response.h"

#ifndef ICAP_BUFFER_LENGTH
#define ICAP_BUFFER_LENGTH 1024
#endif


namespace icap {

	namespace util {

		/**
		*   Convert a number into a string
		*
		*   @param number number to be converted
		*   @return converted string
		*/
		template <typename T> std::string itoa( T number ) {
			std::ostringstream ss;
			ss << number;

			return ss.str();
		}

		/**
		*   Read a line (ending with \r\n) from the socket
		*
		*   @param socket socket to read from
		*   @param buf buffer to read the data into
		*   @param buf_length length / size of the buffer data is read into
		*   @return number of bytes read
		*/
		int read_line( socketlibrary::TCPSocket * socket, char * buf, int buf_length ) throw();

		/**
		*   split a string into a vector by the given delimiter
		*
		*   @param str input string
		*   @param delimiter (optional) delimiter, defaults to " "
		*/
		std::vector<std::string> split( const std::string &str, const std::string &delimiter = " " ) throw();

		/**
		*   Left trim (trim from start) a passed in string
		*
		*   @param str string to trim
		*   @return trimmed string
		*/
		std::string &ltrim( std::string &str ) throw();

		/**
		*   Right trim (trim from end) a passed in string
		*
		*   @param str string to trim
		*   @return trimmed string
		*/
		std::string &rtrim( std::string &str ) throw();

		/**
		*   Trim (trim from both ends) a passed in string
		*
		*   @param str string to trim
		*   @return trimmed string
		*/
		std::string &trim( std::string &str ) throw();

		/**
		*   Read icap request header from the socket passes in
		*
		*   @param socket socket object to read data from
		*   @return icap request header object
		*/
		icap::RequestHeader * read_req_header( socketlibrary::TCPSocket * socket ) throw();

		/**
		*   Output a response using the passed in icap::Response class to the
		*   passed in socket
		*
		*   @param response response object to get the response data from
		*   @param socket socket object to send the data to
		*/
		void send_response( icap::Response * response, socketlibrary::TCPSocket * socket ) throw();

		/**
		*   Returns the response status text for the given status
		*
		*   @param status status to get the text for
		*/
		const std::string response_status( const ResponseHeader::status_t &status ) throw();

	} /* end of namespace util */

} /* end of namespace icap */

#endif /* !ICAP_UTIL_H */


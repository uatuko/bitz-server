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

#include <psocksxx/iosockstream.h>
#include "request.h"
#include "response.h"

#ifndef ICAP_BUFFER_SIZE
#define ICAP_BUFFER_SIZE 1024
#endif


namespace icap {

	namespace util {

		struct chunk_t {
			unsigned int size;
			std::string  extention;
			std::string  data;
		};

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
		*   Convert a hexadecimal number to a decimal number.
		*
		*   @param hex hex to convert to
		*   @return converted decimal value
		*/
		unsigned int hextodec( const std::string &hex ) throw();

		/**
		*   Convert a decimal number to its hexadecimal value
		*
		*   @param dec decimal number
		*   @return converted hex value
		*/
		const std::string dectohex( const unsigned int &dec ) throw();

		/**
		*   Read a line (ending with \r\n) from the socket
		*
		*   @param socket socket to read from
		*   @param buf buffer to read the data into
		*   @param buf_length length / size of the buffer data is read into
		*   @param incl_endl (optional) switch to control whether to include \r\n in the read line,
		*          default is false.
		*   @return number of bytes read
		*/
		int read_line( psocksxx::iosockstream * socket, char * buf, int buf_length, bool incl_endl = false ) throw();

		/**
		*   Read a line (ending with \r\n) from the socket
		*
		*   @param socket socket instance to read data from
		*   @param incl_endl (optional) switch to control whether to include \r\n in the read line,
		*          default is false.
		*   @return read data
		*/
		std::string read_line( psocksxx::iosockstream * socket, bool incl_endl = false ) throw();

		/**
		*   Read data from the socket
		*
		*   @param socket socket instance to read data from
		*   @param size size / length of data to be read
		*   @return read data
		*/
		std::string read_data( psocksxx::iosockstream * socket, int size ) throw();

		/**
		*   Read chunk size. This is a helper method used by read_chunked().
		*
		*   @param socket socket instance to read from
		*   @return chunk size
		*/
		unsigned int read_chunk_size( psocksxx::iosockstream * socket ) throw();

		/**
		*   Read chunk header from the given socket.
		*
		*   @param socket socket instance to read data from
		*   @param chunk chunk data structure to store header data
		*/
		void read_chunk_header( psocksxx::iosockstream * socket, chunk_t &chunk ) throw();

		/**
		*   Read a data chunk from a HTTP chunked transfer encoded data stream.
		*
		*   @param socket socket instance to read data from
		*   @return chunk data structure
		*/
		chunk_t read_chunk( psocksxx::iosockstream * socket ) throw();

		/**
		*   Read chunked data from the given socket
		*
		*   @param socket socket instance to read data from
		*   @return read data (without the control characters)
		*/
		std::string read_chunked( psocksxx::iosockstream * socket ) throw();

		/**
		*   Read chunked payload data from the given socket
		*
		*   @param socket socket instance to read data from
		*   @param payload payload to read data into
		*   @return boolean flag to denote the presence of "ieof"
		*/
		bool read_chunked_payload( psocksxx::iosockstream * socket, std::string &payload ) throw();

		/**
		*   Send / write a line (ending with \r\n) to the socket
		*
		*   @param line line to send through the socket without the line-ending chars
		*   @param socket socket object to write the data to
		*   @return boolean to denote success or failure
		*/
		bool send_line( const std::string &line, psocksxx::iosockstream * socket ) throw();

		/**
		*   Send / write data to the socket.
		*   If chunked is set to true then data will be transferred using
		*   "chunked" transfer-encoding.
		*
		*   @param data data to be sent
		*   @param socket socket instance to write to
		*   @return boolean to denote success or failure
		*/
		bool send_data( const std::string &data, psocksxx::iosockstream * socket ) throw();

		/**
		*   Send / write data to the socket using chunked transfer encoding
		*
		*   @param data data to be sent
		*   @param socket socket instance to write to
		*   @return boolean to denote success or failure
		*/
		bool send_chunked( const std::string &data, psocksxx::iosockstream * socket ) throw();

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
		icap::RequestHeader * read_req_header( psocksxx::iosockstream * socket ) throw();

		/**
		*   Read icap request into the icap::Request instance
		*   using the socket passed in
		*
		*   @param request request object to read data into
		*   @param socket socket object to read data from
		*   @return boolean to denote success or failure
		*/
		bool read_req_data( icap::Request * request, psocksxx::iosockstream * socket ) throw();

		/**
		*   Read icap request data after a '100 Continue' response. This will not look for any
		*   additional headers and will treat any data coming through the socket as payload data.
		*
		*   @param request request object to read data into
		*   @param socket socket object to read data from
		*   @return boolean to denote success or failure
		*/
		bool read_req_continue_data( icap::Request * request, psocksxx::iosockstream * socket ) throw();

		/**
		*   Send / write header data to a socket
		*
		*   @param headers headers to be sent
		*   @param socket socket object to write the data to
		*   @return boolean to denote success or failure
		*/
		bool send_headers( icap::Header::headers_t headers, psocksxx::iosockstream * socket ) throw();

		/**
		*   Output a response using the passed in icap::Response class to the
		*   passed in socket
		*
		*   @param response response object to get the response data from
		*   @param socket socket object to send the data to
		*   @return boolean to denote success or failure
		*/
		bool send_response( icap::Response * response, psocksxx::iosockstream * socket ) throw();

		/**
		*   Returns the response status text for the given status
		*
		*   @param status status to get the text for
		*/
		const std::string response_status( const ResponseHeader::status_t &status ) throw();

	} /* end of namespace util */

} /* end of namespace icap */

#endif /* !ICAP_UTIL_H */


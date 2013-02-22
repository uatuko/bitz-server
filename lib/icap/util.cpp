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
#include <algorithm>
#include <locale>
#include <iostream>


namespace icap {

	namespace util {

		int read_line( socketlibrary::TCPSocket * socket, char * buf, int buf_length, bool incl_endl ) throw() {

			int i  = 0, n;
			char c = '\0';

			while ( i < ( buf_length - 1 ) ) {

				n = socket->recv( &c, 1 );

				if ( n > 0 ) {
					if ( c == '\r' ) {

						if ( incl_endl ) {
							buf[i] = c;
							i++;
						}

						// peak for \n
						n = socket->peek( &c, 1 );

						if ( ( n > 0 ) && ( c == '\n' ) ) {

							n = socket->recv( &c, 1 );

							if ( incl_endl ) {
								buf[i] = c;
								i++;
							}

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


		bool send_line( const std::string &line, socketlibrary::TCPSocket * socket ) throw() {

			try {
				socket->send( line.c_str(), line.length() );
				socket->send( "\r\n", 2 );
			} catch ( socketlibrary::SocketException &sex ) {
				// TODO: log errors
				return false;
			}

			return true;

		}


		bool send_data( const std::string &data, socketlibrary::TCPSocket * socket, bool chunked ) throw() {

			try {

				if ( chunked ) {
					// TODO: chunked transfer
				} else {
					socket->send( data.c_str(), data.size() );
				}

			} catch( socketlibrary::SocketException &sex ) {
				// TODO: log errors
				return false;
			}

			return true;

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
			std::string data = "";

			while ( ( n = read_line( socket, buffer, ICAP_BUFFER_LENGTH, true ) ) > 2 ) {
				data.append( buffer );
			}

			icap::RequestHeader * req_header = new icap::RequestHeader( data );
			return req_header;

		}


		bool read_req_data( icap::Request * request, socketlibrary::TCPSocket * socket ) throw() {

			int data_offset = 0;
			int data_length = 0;
			int data_read   = 0;
			std::vector<icap::Header::encapsulated_header_data_t> sorted_encaps_header;
			std::vector<icap::Header::encapsulated_header_data_t>::iterator sorted_idx;

			// payload
			icap::payload_t payload;
			payload.req_header = "";
			payload.req_body   = "";
			payload.res_header = "";
			payload.res_body   = "";

			// header
			icap::Header * header = request->header();
			sorted_encaps_header  = header->sort_encapsulated_header();

			// loop through the sorted header, for first to (last - 1)
			for ( sorted_idx = sorted_encaps_header.begin(); sorted_idx != ( sorted_encaps_header.end() - 1 ); sorted_idx++ ) {

				// don't want to read negative headers
				if ( sorted_idx->second < 0 ) {
					continue;
				}

				data_offset = sorted_idx->second;
				data_length = ( ( sorted_idx + 1 )->second - data_offset );


				/* read request data */

				// is there anything to read?
				if ( data_length > 0  ) {

					char buffer[data_length];

					// read from the socket
					data_read = socket->recv( buffer, data_length );

					// sanity check
					if ( data_read != data_length ) {
						// something is not right
						return false;
					}

					// end char buffer
					buffer[data_read] = NULL;

					// update payload
					if ( sorted_idx->first == "req-hdr" ) {
						payload.req_header = buffer;
					} else if (  sorted_idx->first == "req-body" ) {
						payload.req_body   = buffer;
					} else if (  sorted_idx->first == "res-hdr" ) {
						payload.res_header = buffer;
					} else if (  sorted_idx->first == "res-body" ) {
						payload.res_body   = buffer;
					} else {
						// TODO: error?
					}

				}

			}

			// update request
			request->payload( payload );

			return true;

		}


		bool send_headers( icap::Header * header, socketlibrary::TCPSocket * socket ) throw() {

			std::string line;
			icap::Header::headers_index_t i;
			icap::ResponseHeader::headers_t headers;

			// headers
			headers = header->headers();

			for ( i = headers.begin(); i != headers.end(); i++ ) {

				line = i->first;
				line.append( ": " );
				line.append( i->second );

				if (! send_line( line, socket ) ) {
					return false;
				}

			}

			// send encapsulated header
			line = "Encapsulated: ";
			line.append( header->encapsulated_header_str() );
			if (! send_line( line, socket ) ) {
				return false;
			}

			return true;

		}


		bool send_response( icap::Response * response, socketlibrary::TCPSocket * socket ) throw() {

			bool r_success = true;

			icap::ResponseHeader * header;

			// grab the response header
			header = response->header();

			// response status
			std::string line = header->protocol();
			line.append( " " );
			line.append( itoa( header->status() ) );
			line.append( " " );
			line.append( response_status( header->status() ) );

			r_success = send_line( line, socket );

			// response headers
			if ( r_success ) {
				r_success = send_headers( header, socket );
			}

			// response content (if there are any)
			if ( r_success ) {

				// req-hdr
				if ( response->payload().req_header.size() > 0 ) {
					send_data( response->payload().req_header, socket );
				}

				// red-body
				if ( response->payload().req_body.size() > 0 ) {
					send_data( response->payload().req_body, socket, true );
				}

				// res-hdr
				if ( response->payload().res_header.size() > 0 ) {
					send_data( response->payload().res_header, socket );
				}

				// res-body
				if ( response->payload().res_body.size() > 0 ) {
					send_data( response->payload().res_body, socket, true );
				}

			}

			return r_success;

		}


		const std::string response_status( const ResponseHeader::status_t &status ) throw() {

			// FIXME: probably there's a better way of mapping this
			std::map<ResponseHeader::status_t, std::string> status_text;

			status_text[ResponseHeader::CONTINUE]           = "Continue";
			status_text[ResponseHeader::OK]                 = "OK";
			status_text[ResponseHeader::NO_CONTENT]         = "No modifications needed";
			status_text[ResponseHeader::BAD_REQUEST]        = "Bad request";
			status_text[ResponseHeader::NOT_FOUND]          = "ICAP Service not found";
			status_text[ResponseHeader::NOT_ALLOWED]        = "Method not allowed for service";
			status_text[ResponseHeader::REQ_TIMEOUT]        = "Request timeout";
			status_text[ResponseHeader::SERVER_ERROR]       = "Server error";
			status_text[ResponseHeader::NOT_IMPLEMENTED]    = "Method not implemented";
			status_text[ResponseHeader::BAD_GATEWAY]        = "Bad gateway";
			status_text[ResponseHeader::SERVICE_OVERLOADED] = "Service overloaded";
			status_text[ResponseHeader::NOT_SUPPORTED]      = "ICAP version not supported by server";

			return status_text[status];

		}

	} /* end of namespace util */

} /* end of namespace icap */


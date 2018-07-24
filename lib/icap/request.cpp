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

#include "request.h"

#include <cstdlib>


namespace icap {

	Request::Request() : _header( 0 ) {
		//
	}


	Request::Request( RequestHeader * req_header ) {

		_header  = req_header;

		// initialise defaults
		_payload.req_header = "";
		_payload.req_body   = "";
		_payload.res_header = "";
		_payload.res_body   = "";
		_payload.ieof       = false;

	}


	Request::~Request() { }


	RequestHeader * const Request::header() const throw() {
		return _header;
	}


	void Request::payload( payload_t payload ) throw() {

		_payload = payload;

		// update encapsulated data
		_header->update_encapsulated( _payload );

	}


	const payload_t &Request::payload() const throw() {
		return _payload;
	}


	const int Request::preview_size() throw() {

		int size = -1;

		// grab the size from request header
		std::string s_size = _header->value( "Preview" );

		// sanity check
		if (! s_size.empty() ) {

			// convert string to integer
			size = atoi( s_size.c_str() );

		}

		return size;

	}


	void Request::read( char* buf, size_t size ) {
		if ( !_header ) {
			// read header
			char c = _data.back();
			auto lendl = _data.rfind( "\r\n" );

			for ( size_t idx = 0; idx < size; idx++ ) {
				_data += buf[idx];

				// look for \r\n
				if ( c == '\r' && buf[idx] == '\n' ) {
					// if we have \r\n\r\n then that's the end of header
					if ( lendl != std::string::npos && ( ( _data.size() - lendl ) == 4 ) ) {
						// end of header
						// FIXME: use smart pointers
						_header = new RequestHeader( _data );

						// done reading header, continue with body if there's more data
						idx++;
						return read_body( ( buf + idx ), size - idx );
					} else {
						lendl = _data.size() - 2;  // update last \r\n index
					}
				}

				c = buf[idx];
			}

		} else {
			// read body
			read_body( buf, size );
		}
	}


	void Request::read_body( char* buf, size_t size ) {
		// TODO:
	}

} /* end of namespace icap */


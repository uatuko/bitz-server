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

	Request::Request( RequestHeader * req_header ) {

		_header  = req_header;

		// initialise defaults
		_payload.req_header = "";
		_payload.req_body   = "";
		_payload.res_header = "";
		_payload.res_body   = "";

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

} /* end of namespace icap */


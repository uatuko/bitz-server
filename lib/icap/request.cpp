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


namespace icap {

	Request::Request( RequestHeader * req_header ) {
		_header  = req_header;
		_payload = "";
	}


	Request::~Request() { }


	RequestHeader * const Request::header() const throw() {
		return _header;
	}


	const std::string &Request::payload() const throw() {
		return _payload;
	}


	void Request::payload( const char * payload ) throw() {
		_payload = payload;
	}

} /* end of namespace icap */


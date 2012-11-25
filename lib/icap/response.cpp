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

#include "response.h"


namespace icap {

	Response::Response( ResponseHeader * response_header ) {

		if ( response_header != NULL ) {
			_response_header = response_header;
			_cleanup_header  = false;
		} else {
			_response_header = new ResponseHeader( ResponseHeader::SERVER_ERROR );
			_cleanup_header  = true;
		}

	}


	Response::Response( ResponseHeader::status_t status ) {
		_response_header = new ResponseHeader( ResponseHeader::SERVER_ERROR );
		_cleanup_header  = true;
	}


	Response::~Response() {

		// cleanup
		if ( _cleanup_header ) {
			delete _response_header;
		}

	}

} /* end of namespace icap */


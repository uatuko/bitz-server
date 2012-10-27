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

#include "request_header.h"
#include "util.h"


namespace icap {

	/*
	*  sample icap request:
	*  REQMOD icap://icap-server.net/server?arg=87 ICAP/1.0
	*  Host: icap-server.net
	*  Encapsulated: req-hdr=0, null-body=170
	*
	*/
	RequestHeader::RequestHeader( const std::vector<std::string> &data ) : Header() {

		if ( data.size() > 0 ) {

			std::vector<std::string> request = util::split( data.at( 0 ) );

			if ( request.size() == 3 ) {
				_request.method   = request.at(0);
				_request.uri      = request.at(1);
				_request.protocol = request.at(2);
			} else {
				// TODO: error, invalid request format
			}

		} else {
			_request.method   = "";
			_request.uri      = "";
			_request.protocol = "ICAP/1.0";
		}

	}


	RequestHeader::~RequestHeader() { }

} /* end of namespace icap */


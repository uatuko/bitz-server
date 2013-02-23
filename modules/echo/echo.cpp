/*
 *	bitz-server, An ICAP server implementation in C++
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

#include "echo.h"


namespace bitz {

	Echo::Echo() : Modifier() { }
	Echo::~Echo() { }


	icap::Response * Echo::modify( icap::Request * request ) throw() {

		icap::Response * response;
		icap::payload_t payload;

		// copy payload from request
		payload.req_header = request->payload().req_header;
		payload.req_body   = request->payload().req_body;
		payload.res_header = request->payload().res_header;
		payload.res_body   = request->payload().res_body;


		response = new icap::Response( icap::ResponseHeader::OK );
		response->payload( payload );

		return response;

	}


	icap::Response * Echo::preview( icap::Request * request ) throw() {
		// 100 - continue always
		return new icap::Response( icap::ResponseHeader::CONTINUE );
	}

} /* end of namespace bitz */


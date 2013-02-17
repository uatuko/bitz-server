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

#include "response_header.h"
#include "util.h"
#include <config.h>

#include <ctime>
#include <stdlib.h>


namespace icap {

	ResponseHeader::ResponseHeader( status_t status ) : Header() {

		// FIXME: protocol shouldn't be hard-coded
		_response.protocol = "ICAP/1.0";
		_response.status   = status;

		// initialise default headers
		init_defaults();

	}

	ResponseHeader::~ResponseHeader() { }


	const std::string &ResponseHeader::protocol() const throw() {
		return _response.protocol;
	}


	const ResponseHeader::status_t &ResponseHeader::status() const throw() {
		return _response.status;
	}


	void ResponseHeader::init_defaults() throw() {

		update_timestamp();
		generate_istag();
		attach( "Connection" , "close" );
		attach( "Server", PACKAGE_STRING );

		// FIXME: should be using structures
		attach( "Encapsulated", "req-hdr=0, null-body=0" );

	}


	void ResponseHeader::update_timestamp() throw() {

		time_t raw_time;
		struct tm * time_info;
		char buffer [80];

		time( &raw_time );
		time_info = localtime( &raw_time );

		strftime ( buffer, 80, "%c %Z", time_info );
		attach( "Date", buffer );

	}


	void ResponseHeader::generate_istag() throw() {

		time_t raw_time;
		clock_t clock_time;
		std::string istag;

		time( &raw_time );
		clock_time = clock();

		istag = "BITZ-";
		istag.append( util::itoa( raw_time ) ).append( "-" );
		istag.append( util::itoa( clock_time) );

		attach( "ISTag", istag.substr( 0, 32 ) );

	}

} /* end of namespace icap */


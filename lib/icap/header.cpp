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

#include "header.h"
#include "util.h"

#include <iostream>
#include <stdlib.h>


namespace icap {

	Header::Header() {

		// initialise defaults
		_encapsulated["req-hdr"]   = -1;
		_encapsulated["req-body"]  = -1;
		_encapsulated["res-hdr"]   = -1;
		_encapsulated["res-body"]  = -1;
		_encapsulated["opt-body"]  = -1;
		_encapsulated["null-body"] = -1;

	}

	Header::~Header() {}


	const Header::headers_t &Header::headers() const throw() {
		return _headers;
	}


	Header::encapsulated_header_t Header::encapsulated_header() const throw() {
		return _encapsulated;
	}


	void Header::attach( std::string key, std::string value ) throw() {

		// trim
		key   = util::trim( key );
		value = util::trim( value );

		// check for 'Encapsulated' headers
		if ( key == "Encapsulated" ) {
			attach_encapsulated( value );
		} else {
			_headers[key] = value;
		}

	}


	bool Header::attach_encapsulated( std::string header_value ) throw() {

		std::vector<std::string> list_data;
		std::vector<std::string> entity_data;

		encapsulated_header_index_t idx;
		bool r_status = true;

		// grab the entity list [ req-hdr=0, null-body=170 ]
		list_data = util::split( util::trim( header_value ), "," );

		for ( size_t i = 0; i < list_data.size(); i++ ) {

			// get entity data [ req-hdr=0 ]
			entity_data = util::split( util::trim( list_data.at( i ) ), "=" );

			if ( entity_data.size() == 2 ) {

				idx = _encapsulated.find( util::trim( entity_data.at( 0 ) ) );
				if ( idx != _encapsulated.end() ) {
					idx->second = atoi( util::trim( entity_data.at( 1 ) ).c_str() );
				}

			} else {
				r_status = false;
			}
		}

		return r_status;

	}


	bool Header::remove( std::string key ) throw() {
		return ( (bool) _headers.erase( util::trim( key ) ) );
	}


	const std::string Header::encapsulated_header_str() throw() {

		/*
		*  Encapsulated request header grammer:
		*   REQMOD  request  encapsulated_list: [reqhdr] reqbody
		*   REQMOD  response encapsulated_list: {[reqhdr] reqbody} |
		*                                       {[reshdr] resbody}
		*   RESPMOD request  encapsulated_list: [reqhdr] [reshdr] resbody
		*   RESPMOD response encapsulated_list: [reshdr] resbody
		*   OPTIONS response encapsulated_list: optbody
		*/

		// TODO:
		return "";

	}

}


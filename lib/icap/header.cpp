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
		_encapsulated.null_body = 0;
		_encapsulated.opt_body  = 0;
		_encapsulated.req_body  = 0;
		_encapsulated.res_body  = 0;
		_encapsulated.req_hdr   = 0;
		_encapsulated.res_hdr   = 0;

		// populate mappings
		populate_encapsulated_map();

	}

	Header::~Header() {}


	const Header::headers_t &Header::headers() const throw() {
		return _headers;
	}


	const Header::encapsulated_header_t &Header::encapsulated_header() const throw() {
		return _encapsulated;
	}


	void Header::attach( std::string key, std::string value ) throw() {

		// trim
		key   = util::trim( key );
		value = util::trim( value );

		// check for 'Encapsulated' headers
		if ( key == "Encapsulated" ) {
			attach_encapsulated( value );
		}

		_headers[key] = value;

	}


	void Header::attach_encapsulated( std::string header_value ) throw() {

		std::vector<std::string> list_data;
		std::vector<std::string> entity_data;

		_encapsulated_map_index_t idx;

		// grab the entity list [ req-hdr=0, null-body=170 ]
		list_data = util::split( util::trim( header_value ), "," );

		for ( size_t i = 0; i < list_data.size(); i++ ) {

			// get entity data [ req-hdr=0 ]
			entity_data = util::split( util::trim( list_data.at( i ) ), "=" );

			if ( entity_data.size() == 2 ) {

				idx = _encapsulated_map.find( util::trim( entity_data.at( 0 ) ) );
				if ( idx != _encapsulated_map.end() ) {
					*idx->second = atoi( util::trim( entity_data.at( 1 ) ).c_str() );
				}

			} else {
				// TODO: error parsing the header
			}
		}

	}


	bool Header::remove( std::string key ) throw() {
		return ( (bool) _headers.erase( util::trim( key ) ) );
	}


	void Header::populate_encapsulated_map() throw() {

		_encapsulated_map["req-hdr"]   = &_encapsulated.req_hdr;
		_encapsulated_map["req-body"]  = &_encapsulated.req_body;
		_encapsulated_map["res-hdr"]   = &_encapsulated.res_hdr;
		_encapsulated_map["res-body"]  = &_encapsulated.res_body;
		_encapsulated_map["opt-body"]  = &_encapsulated.opt_body;
		_encapsulated_map["null-body"] = &_encapsulated.null_body;

	}

}


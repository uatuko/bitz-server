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

#include <algorithm>
#include <cstdlib>


namespace icap {

	Header::Header() {

		// initialise encapsulated entity list
		_encapsel.push_back( &_encapsulated.req_header );
		_encapsel.push_back( &_encapsulated.req_body );
		_encapsel.push_back( &_encapsulated.res_header );
		_encapsel.push_back( &_encapsulated.res_body );
		_encapsel.push_back( &_encapsulated.opt_body );
		_encapsel.push_back( &_encapsulated.null_body );

	}

	Header::~Header() {}


	const Header::headers_t &Header::headers() const throw() {
		return _headers;
	}


	const std::string Header::value( const std::string &key ) throw() {

		std::string value           = "";
		Header::headers_index_t idx = _headers.find( key );

		if ( idx != _headers.end() ) {
			value = idx->second;
		}

		return value;

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

		bool r_status = true;

		// grab the entity list [ req-hdr=0, null-body=170 ]
		list_data = util::split( util::trim( header_value ), "," );

		for ( size_t i = 0; i < list_data.size(); i++ ) {

			// get entity data [ req-hdr=0 ]
			entity_data = util::split( util::trim( list_data.at( i ) ), "=" );

			if ( entity_data.size() == 2 ) {

				auto entity = util::trim( entity_data.at( 0 ) );
				for ( encapsulated_entity_t* e : _encapsel ) {
					if ( e->name == entity ) {
						e->offset = atoi( util::trim( entity_data.at( 1 ) ).c_str() );
						e->valid = true;
						break;
					}
				}

			} else {
				r_status = false;
			}
		}


		// sort encapsulated entiry list
		_encapsel.sort( encapsulated_entity_compare );

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

		std::string encaps_header = "";

		for ( encapsulated_entity_t* e : _encapsel ) {
			if ( e->valid ) {
				if ( encaps_header != "" ) {
					encaps_header.append( ", " );
				}

				encaps_header.append( e->name ).append( "=" ).append( util::itoa( e->offset ) );
			}
		}

		// sanity check
		if ( encaps_header == "" ) {
			encaps_header = "null-body=0";
		}

		return encaps_header;

	}


	void Header::update_encapsulated( const payload_t &payload ) throw() {

		unsigned int data_length = 0;
		unsigned int data_offset = 0;

		// request header
		if ( payload.req_header.size() > 0 ) {
			_encapsulated.req_header.offset = data_length;
			data_offset = data_length;
			data_length += payload.req_header.size();
		}

		// request body (POST data)
		if ( payload.req_body.size() > 0 ) {
			_encapsulated.req_body.offset = data_length;
			data_offset = data_length;
			data_length += payload.req_body.size();
		}

		// response header
		if ( payload.res_header.size() > 0 ) {
			_encapsulated.res_header.offset = data_length;
			data_offset = data_length;
			data_length += payload.res_header.size();
		}

		// response body
		if ( payload.res_body.size() > 0 ) {
			_encapsulated.res_body.offset = data_length;
			data_offset = data_length;
			data_length += payload.res_body.size();
		}

		// null-body
		if ( data_offset == 0 ) {
			_encapsulated.null_body.offset = data_length;
		}


		// sort encapsulated entity list
		_encapsel.sort( encapsulated_entity_compare );

	}


	std::vector<Header::encapsulated_header_data_t> Header::sort_encapsulated_header() {

		// FIXME: use std::list
		std::vector<Header::encapsulated_header_data_t> data;
		for ( auto e : _encapsel ) {
			data.push_back( encapsulated_header_data_t{ e->name, ( e->valid ? e->offset : -1 ) } );
		}

		return data;

	}

}


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


	const std::string Header::value( const std::string &key ) throw() {

		std::string value           = "";
		Header::headers_index_t idx = _headers.find( key );

		if ( idx != _headers.end() ) {
			value = idx->second;
		}

		return value;

	}


	const int Header::encapsulated_header( const std::string &entity ) throw() {

		Header::encapsulated_header_index_t idx;

		idx = _encapsulated.find( entity );
		if ( idx == _encapsulated.end() ) {
			return -1;
		}

		return idx->second;

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

		Header::encapsulated_header_index_t idx;
		std::string encaps_header = "";

		// FIXME: chances are that we will always get the correct order
		//        but should consider sorting
		for ( idx = _encapsulated.begin(); idx != _encapsulated.end(); idx++ ) {

			if ( idx->second > 0 ) {

				if ( encaps_header != "" ) {
					encaps_header.append( ", " );
				}

				encaps_header.append( idx->first ).append( "=" ).append( util::itoa( idx->second ) );

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
			_encapsulated["req-hdr"] = data_length;
			data_offset = data_length;
			data_length += payload.req_header.size();
		}

		// request body (POST data)
		if ( payload.req_body.size() > 0 ) {
			_encapsulated["req-body"] = data_length;
			data_offset = data_length;
			data_length += payload.req_body.size();
		}

		// response header
		if ( payload.res_header.size() > 0 ) {
			_encapsulated["res-hdr"] = data_length;
			data_offset = data_length;
			data_length += payload.res_header.size();
		}

		// response body
		if ( payload.res_body.size() > 0 ) {
			_encapsulated["res-body"] = data_length;
			data_offset = data_length;
			data_length += payload.res_body.size();
		}

		// null-body
		if ( data_offset == 0 ) {
			_encapsulated["null-body"] = data_length;
		}

	}


	std::vector<Header::encapsulated_header_data_t> Header::sort_encapsulated_header() {

		std::vector<Header::encapsulated_header_data_t> data( _encapsulated.begin(), _encapsulated.end() );
		std::sort(data.begin(), data.end(), encapsulated_header_compare());

		return data;

	}

}


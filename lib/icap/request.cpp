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

	Request::Request() : _header( 0 ) {
		//
	}


	Request::Request( RequestHeader * req_header ) {
		_header  = req_header;
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


	void Request::read( const char* buf, size_t size ) {
		if ( !_header ) {
			// read header
			char c = _data.back();
			auto lendl = _data.rfind( "\r\n" );

			for ( size_t idx = 0; idx < size; idx++ ) {
				_data += buf[idx];

				// look for \r\n
				if ( c == '\r' && buf[idx] == '\n' ) {
					// if we have \r\n\r\n then that's the end of header
					if ( lendl != std::string::npos && ( ( _data.size() - lendl ) == 4 ) ) {
						// end of header

						// FIXME: use smart pointers
						_header = new RequestHeader( _data );

						// done reading header, continue with body if there's more data
						idx++;
						return read_payload( ( buf + idx ), size - idx );
					} else {
						lendl = _data.size() - 2;  // update last \r\n index
					}
				}

				c = buf[idx];
			}

		} else {
			// read body
			read_payload( buf, size );
		}
	}


	void Request::read_payload( const char* buf, size_t size ) {

		size_t bytes = 0;
		auto encapsel = _header->encapsel();

		for ( auto it = encapsel.begin(); it != encapsel.end(); it++ ) {
			auto nit = std::next( it );
			auto e = *it;

			if ( e->offset > _payload.offset ) {
				continue;
			}

			if ( nit == encapsel.end() ) {
				bytes = size;
			} else {
				bytes = (*nit)->offset - _payload.offset;
				if ( bytes > size ) { bytes = size; }
			}

			/* read payload data */
			// if this is the last entity, then check for chunked content
			if ( nit == encapsel.end() ) {
				if ( e->name == "req-body" ) {
					// TODO: read chunked
				} else if ( e->name == "res-body" ) {
					// TODO: read chunked
				} else {
					// null-body is the only other valid possibility
				}
			} else {
				if ( e->name == "req-hdr" ) {
					_payload.req_header.append( buf, bytes );
				} else if ( e->name == "req-body" ) {
					_payload.req_body.append( buf, bytes );
				} else if ( e->name == "res-hdr" ) {
					_payload.res_header.append( buf, bytes );
				} else if ( e->name == "res-body" ) {
					_payload.res_body.append( buf, bytes );
				}
			}

			// update counters
			_payload.offset += bytes;
			size -= bytes;

			if ( size == 0 ) {
				// no more data to read
				break;
			} else {
				// update pointer to the buffer for next read cycle
				buf += bytes;
			}
		}

	}

} /* end of namespace icap */


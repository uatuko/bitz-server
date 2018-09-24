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
#include "util.h"


namespace icap {

	Request::Request() : _header( 0 ), _status( status_t::unknown ) {
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
			size = std::stoi( s_size );

		}

		return size;

	}


	void Request::read( const char* buf, size_t size ) {
		if ( _status == status_t::unknown && !_header ) {
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

						// FIXME: use smart pointers (or no pointers), this is leaking memory
						_header = new RequestHeader( _data );
						_status = status_t::header;
						_data   = "";

						// done reading header, continue with body if there's more data
						idx++;
						return read_payload( ( buf + idx ), size - idx );
					} else {
						lendl = _data.size() - 2;  // update last \r\n index
					}
				}

				c = buf[idx];
			}

		} else if ( _status >= status_t::header && _status < status_t::eor ) {
			// read body
			read_payload( buf, size );
		} else {
			// TODO: error?
		}
	}


	Request::status_t Request::status() {
		return _status;
	}


	void Request::read_payload( const char* buf, size_t size ) {

		// sanity checks
		if ( size == 0 ) {
			return;
		}

		size_t bytes = 0;
		auto encapsel = _header->encapsel();

		for ( auto it = encapsel.begin(); it != encapsel.end(); it++ ) {
			auto nit = std::next( it );
			auto e = *it;

			if ( nit == encapsel.end() ) {
				bytes = size;
			} else {
				if ( _payload.offset >= (*nit)->offset ) {
					continue;
				}

				bytes = (*nit)->offset - _payload.offset;
				if ( bytes > size ) { bytes = size; }
			}

			/* read payload data */
			_status = status_t::payload;

			// if this is the last entity, then check for chunked content
			if ( nit == encapsel.end() ) {
				auto b = bytes;
				while ( b > 0 ) {
					const chunk_t chunk = read_chunked( buf + ( bytes - b ), b );
					if ( chunk.status == chunk_status_t::eoc ) {
						if ( e->name == "req-body" ) {
							_payload.req_body += chunk.data;
						} else if ( e->name == "res-body" ) {
							_payload.res_body += chunk.data;
						} else {
							// null-body is the only other valid possibility
						}

						if ( chunk.extension == "ieof" ) {
							_payload.ieof = true;
						}

						_status = status_t::eor;
					}

					b = chunk.overflow;
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
				if ( ( _payload.offset == (*nit)->offset ) && ( (*nit)->name == "null-body" ) ) {
					_status = status_t::eor;
				}
				break;
			} else {
				// update pointer to the buffer for next read cycle
				buf += bytes;
			}
		}

	}


	const chunk_t &Request::read_chunked( const char* buf, size_t size ) {
		if ( _chunk.status == chunk_status_t::eoc ) {
			// this is a new chunk
			_chunk = chunk_t{};
			_data  = "";
		}

		if ( _chunk.size == 0 && _chunk.status == chunk_status_t::unknown ) {
			size_t offset = seek_endl( buf, size, _data.back() );
			if ( offset > 0 ) {
				size_t i = seekc( buf, ( offset - 2 ), ';' );
				if ( i > 0 ) {
					_chunk.size = util::hextodec( std::string( buf, i ) );
					std::string s = std::string( ( buf + i ), ( offset - 2 - i ) );
					_chunk.extension = util::trim( s );
				} else {
					_chunk.size = util::hextodec( std::string( buf, ( offset - 2 ) ) );
				}

				_data = "";
				_chunk.status = chunk_status_t::partial;
			} else {
				if ( offset == 0 ) { offset = size; }
				_data.append( buf, offset );
			}

			buf += offset;
			size -= offset;
		}

		if ( _chunk.status == chunk_status_t::unknown ) {
			// there won't be any overflow at this point
			return _chunk;
		}


		size_t bytes = ( _chunk.size - _chunk.data.size() );
		if ( bytes > size ) {
			bytes = size;
		};

		_chunk.data.append( buf, bytes );
		buf += bytes;
		size -= bytes;

		if ( ( _chunk.size - _chunk.data.size() ) <= 0 )  {
			size_t offset = seek_endl( buf, size, _data.back() );
			if ( offset > 0 ) {
				// FIXME: offset can't be > 2 here, should that be checked?
				if ( _chunk.size == _chunk.data.size() ) {
					_chunk.status = chunk_status_t::eoc;
				} else {
					// FIXME: error reading chunk
				}

				_data = "";
			} else {
				// adjust offset, it'll be 0 when we get here
				offset = size;
				_data.append( buf, offset );
			}

			buf += offset;
			size -= offset;
		}

		_chunk.overflow = size;
		return _chunk;
	}


	size_t Request::seek_endl( const char* buf, size_t size, char c ) {
		size_t offset = 0;
		for ( size_t i = 0; i < size; i++ ) {
			if ( c == '\r' && buf[i] == '\n' ) {
				offset = i + 1;
				break;
			}

			c = buf[i];
		}

		return offset;
	}


	size_t Request::seekc( const char* buf, size_t size, char c ) {
		size_t offset = 0;
		for ( size_t i = 0; i < size; i++ ) {
			if ( buf[i] == c ) {
				offset = i + 1;
				break;
			}
		}

		return offset;
	}

} /* end of namespace icap */


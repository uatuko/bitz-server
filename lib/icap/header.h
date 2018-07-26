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

#ifndef ICAP_HEADER_H
#define ICAP_HEADER_H

#include "common.h"

#include <string>
#include <map>
#include <vector>
#include <list>
#include <functional>


namespace icap {

	class Header {
	public:

		/* headers data type */
		typedef std::map<std::string, std::string> headers_t;

		/* headers iterator type */
		typedef headers_t::iterator headers_index_t;

		/* encapsulated header data type */
		typedef std::pair<std::string, int> encapsulated_header_data_t;

		struct encapsulated_entity_t {
			std::string name;
			size_t offset;
			bool valid;

			encapsulated_entity_t() : offset( 0 ), valid( false ) {}

			bool operator <( const encapsulated_entity_t &e ) const {
				if ( valid && e.valid ) {
					return offset < e.offset;
				}

				return valid;
			}
		};

		struct encapsulated_header_t {
			encapsulated_entity_t req_header;
			encapsulated_entity_t res_header;
			encapsulated_entity_t req_body;
			encapsulated_entity_t res_body;
			encapsulated_entity_t opt_body;
			encapsulated_entity_t null_body;

			encapsulated_header_t() {
				req_header.name = "req-hdr";
				res_header.name = "res-hdr";
				req_body.name = "req-body";
				res_body.name = "res-body";
				opt_body.name = "opt-body";
				null_body.name = "null-body";
			}
		};


		Header();
		virtual ~Header();

		/**
		*   Return headers
		*   @return headers
		*/
		const headers_t &headers() const throw();

		/**
		*   Returns the header value for the given header key or an empty string
		*   if the header is not found.
		*
		*   @param key header key
		*   @return header value
		*/
		const std::string value( const std::string &key ) throw();

		/**
		*   Attach header data into the header
		*
		*   <pre>
		*   e.g.
		*   Host: icap-server.net
		*   Encapsulated: req-hdr=0, null-body=170
		*   [key]: [value]
		*   </pre>
		*
		*   @param key header key
		*   @param value header value
		*/
		virtual void attach( std::string key, std::string value ) throw();

		/**
		*   Attach Encapsulated header data. This method should only be used
		*   when reading a raw request / response. Consider using update_encapsulated()
		*   method for other scenarios.
		*
		*   <pre>
		*   e.g.
		*   Encapsulated: req-hdr=0, req-body=412
		*   Encapsulated: req-hdr=0, res-hdr=822, res-body=1655
		*   Encapsulated: [header_value]
		*   </pre>
		*
		*   @param header_value Encapsulated header value
		*   @return boolean to denote success or failure
		*/
		virtual bool attach_encapsulated( std::string header_value ) throw();


		/**
		*   Update Encapsulated header data using the passed in (icap::payload_t) payload.
		*   When the request / response has been populated with the payload, calling this
		*   method will update the encapsulated header entities with appropriate values.
		*
		*   This methos will always succeed.
		*
		*   @param payload request or response payload
		*/
		virtual void update_encapsulated( const payload_t &payload ) throw();

		/**
		*   Remove header data from the header instance with the given key
		*
		*   @param key header key
		*   @return boolean to denote success or failure
		*/
		virtual bool remove( std::string key ) throw();

		/**
		*   Return Encapsulated header as a std::string value.
		*   @return encapsulated header value (e.g. res-hdr=0, res-body=213)
		*/
		virtual const std::string encapsulated_header_str() throw();

		/**
		*   Sort the encapsulated header data and return a std::vector of
		*   encapsulated_header_data_t. The actual header data won't be changed.
		*
		*   @return sorted encapsulated header
		*/
		virtual std::vector<encapsulated_header_data_t> sort_encapsulated_header();

		virtual std::list<encapsulated_entity_t*> encapsel();


	protected:
		headers_t _headers;
		encapsulated_header_t _encapsulated;
		std::list<encapsulated_entity_t*> _encapsel;

	private:

		static bool encapsulated_entity_compare( const encapsulated_entity_t* a, const encapsulated_entity_t* b ) {
			if ( a->valid && b->valid ) {
				return a->offset < b->offset;
			}

			return a->valid;
		}

	};

} /* end of namespace icap */

#endif /* !ICAP_HEADER_H */


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

#include <string>
#include <map>
#include <vector>

namespace icap {

	class Header {
	public:

		/* headers data type */
		typedef std::map<std::string, std::string> headers_t;

		/* headers iterator type */
		typedef headers_t::iterator headers_index_t;

		/* encapsulated header type */
		typedef std::map<std::string, int> encapsulated_header_t;

		/* encapsulated header iterator type */
		typedef encapsulated_header_t::iterator encapsulated_header_index_t;


		Header();
		virtual ~Header();

		/**
		*   Return headers
		*   @return headers
		*/
		const headers_t &headers() const throw();

		/**
		*   Return Encapsulated header
		*   @return encapsulated header
		*/
		encapsulated_header_t encapsulated_header() const throw();

		/**
		*   Attach header data into the header
		*
		*   e.g.
		*   Host: icap-server.net
		*   Encapsulated: req-hdr=0, null-body=170
		*   [key]: [value]
		*
		*   @param key header key
		*   @param value header value
		*/
		virtual void attach( std::string key, std::string value ) throw();

		/**
		*   Attach Encapsulated header data
		*
		*   e.g.
		*   Encapsulated: req-hdr=0, req-body=412
		*   Encapsulated: req-hdr=0, res-hdr=822, res-body=1655
		*   Encapsulated: [header_value]
		*
		*   @param header_value Encapsulated header value
		*   @return boolean to denote success or failure
		*/
		virtual bool attach_encapsulated( std::string header_value ) throw();

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

	protected:
		headers_t _headers;
		encapsulated_header_t _encapsulated;

	private:

	};

} /* end of namespace icap */

#endif /* !ICAP_HEADER_H */


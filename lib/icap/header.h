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

		typedef std::map<std::string, std::string> header_t;

		Header();
		virtual ~Header();


		/**
		*   Return headers
		*   @return headers
		*/
		const header_t &headers() const throw();

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
		*   Remove header data from the header
		*
		*   @param key header key
		*/
		virtual bool remove( std::string key ) throw();

	protected:
		header_t _headers;

	private:

	};

} /* end of namespace icap */

#endif /* !ICAP_HEADER_H */


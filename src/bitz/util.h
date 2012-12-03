/*
 *	bitz-server, An ICAP server implementation in C++
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

#ifndef BITZ_UTIL_H
#define BITZ_UTIL_H

#include "common.h"

#include <string>
#include <sstream>


namespace bitz {

	namespace util {

		/**
		*   Convert a number into a string
		*
		*   @param number number to be converted
		*   @return converted string
		*/
		template <typename T> std::string itoa( T number ) {
			std::ostringstream ss;
			ss << number;

			return ss.str();
		}

		/**
		*   Find a matching request handler for the given method
		*
		*   @param req_handlers request handlers
		*   @param req_method request method
		*   @return request handler (or null pointer if not found)
		*/
		RequestHandler * find_req_handler( req_handlers_t req_handlers, const std::string &req_method ) throw();

	} /* end of namespace util */

} /* end of namespace bitz */

#endif /* !BITZ_UTIL_H */


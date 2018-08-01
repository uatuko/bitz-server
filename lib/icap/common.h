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

#ifndef ICAP_COMMON_H
#define ICAP_COMMON_H

#include <string>


namespace icap {

	/**
	*   Payload structure common to both requests and responses.
	*/
	struct payload_t {
		std::string req_header;    /**< request header  */
		std::string req_body;      /**< request body    */
		std::string res_header;    /**< response header */
		std::string res_body;      /**< response body   */
		bool ieof;                 /**< boolen flag to store the presence of "ieof" */
		size_t offset;             /**< payload read offset    */

		payload_t() : offset( 0 ), ieof( false ) {};
	};

	enum chunk_status_t {
		unknown = -1,
		partial,
		eoc
	};

	struct chunk_t {
		chunk_status_t  status;
		size_t          size;
		size_t          overflow;
		std::string     extension;
		std::string     data;

		chunk_t() : status( chunk_status_t::unknown ), size( 0 ), overflow( 0 ) {};
	};

} /* end of namespace icap */

#endif /* !ICAP_COMMON_H */

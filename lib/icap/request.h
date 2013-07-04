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

#ifndef ICAP_REQUEST_H
#define ICAP_REQUEST_H

#include "common.h"
#include "request_header.h"


namespace icap {

	class Request {
	public:
		Request( RequestHeader * req_header );
		virtual ~Request();

		/**
		*   Return the request header
		*   @return request header
		*/
		RequestHeader * const header() const throw();

		/**
		*   Set the payload data for this response instance from a
		*   icap::payload_t data structure
		*
		*   @param payload payload data structure
		*/
		void payload( payload_t payload ) throw();

		/**
		*   Return the payload data for this response instance
		*   @return payload data
		*/
		const payload_t &payload() const throw();

		/**
		*   Returns the number of preview bytes in the request. If the preview
		*   header is not present in the request then a minus (-1) value will be
		*   returned.
		*   
		*   @return preview bytes
		*/
	   const int preview_size() throw();	

	private:
		RequestHeader * _header;
		payload_t _payload;

	};

} /* end of namespace icap */

#endif /* !ICAP_REQUEST_H */


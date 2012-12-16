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

#include "request_header.h"


namespace icap {

	class Request {
	public:
		Request( RequestHeader * request_header );
		virtual ~Request();

		/**
		*   Return the request header
		*   @return request header
		*/
		RequestHeader * const header() const throw();

	private:
		RequestHeader * _header;

	};

} /* end of namespace icap */

#endif /* !ICAP_REQUEST_H */


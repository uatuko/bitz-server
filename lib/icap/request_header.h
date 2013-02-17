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

#ifndef ICAP_REQUEST_HEADER_H
#define ICAP_REQUEST_HEADER_H

#include "header.h"

#include <string>
#include <vector>


namespace icap {

	class RequestHeader : public Header {
	public:

		struct request_t {
			std::string method;
			std::string uri;
			std::string protocol;
		};

		RequestHeader( const std::string &raw_data );
		virtual ~RequestHeader();

		/**
		*   Return request method
		*   @return method
		*/
		const std::string &method() const throw();

		/**
		*   Return request URI
		*   @return URI
		*/
		const std::string &uri() const throw();

		/**
		*   Return request protocol
		*   @return protocol
		*/
		const std::string &protocol() const throw();

		/**
		*   Return request
		*   @return request
		*/
		const request_t &request() const throw();

		/**
		*   Return raw header data
		*   @return raw request header
		*/
		const std::string &raw_data() const throw();

	private:
		request_t _request;
		std::string _raw_data;

		void read_header( const std::string &raw_data ) throw();

	};

} /* end of namespace icap */

#endif /* !ICAP_REQUEST_HEADER_H */


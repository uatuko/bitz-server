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

#ifndef ICAP_RESPONSE_HEADER_H
#define ICAP_RESPONSE_HEADER_H

#include "header.h"


namespace icap {

	class ResponseHeader : public Header {
	public:

		typedef enum status_t {
			CONTINUE            = 100,
			OK                  = 200,
			NO_CONTENT          = 204,
			BAD_REQUEST         = 400,
			NOT_FOUND           = 404,
			NOT_ALLOWED         = 405,
			REQ_TIMEOUT         = 408,
			SERVER_ERROR        = 500,
			NOT_IMPLEMENTED     = 501,
			BAD_GATEWAY         = 502,
			SERVICE_OVERLOADED  = 503,
			NOT_SUPPORTED       = 505
		};

		struct response_t {
			std::string protocol;
			status_t status;
		};

		ResponseHeader( status_t status );
		virtual ~ResponseHeader();

		/**
		*   Return the response protocol
		*   @return protocol
		*/
		const std::string &protocol() const throw();

		/**
		*   Return the response status
		*   @return status
		*/
		const status_t &status() const throw();

	private:
		response_t _response;

		void update_timestamp() throw();
		void generate_istag() throw();
		void init_defaults() throw();

	};

} /* end of namespace icap */

#endif /* !ICAP_RESPONSE_HEADER_H */


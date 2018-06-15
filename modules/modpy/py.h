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

#ifndef BITZ_PY_H
#define BITZ_PY_H

#include <Python.h>

#include <bitz/modifier.h>
#include <spdlog/spdlog.h>


namespace bitz {

	class Py : public Modifier {
	public:

		struct config_t {
			std::string module_path;
			std::string module_name;
		};

		Py();
		virtual ~Py();

		icap::Response * modify( icap::Request * request ) throw();
		icap::Response * preview( icap::Request * request ) throw();

	private:
		config_t _config;
		PyObject * _pymodule;
		std::shared_ptr<spdlog::logger> _logger;

		void load_configs() throw();
		void init_python() throw();
		void cleanup_python() throw();

		icap::Response * python_response( icap::Request * request, const std::string &method ) throw();

	};

} /* end of namespace bitz */


/* class factories */

extern "C" bitz::Modifier * create() {
	return new bitz::Py;
}

extern "C" void destroy( bitz::Modifier * m ) {
	delete m;
}

#endif /* !BITZ_PY_H */


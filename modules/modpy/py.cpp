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

#include "py.h"
#include "interface.h"


namespace bitz {

	Py::Py() : Modifier() {

		// initialise python
		this->init_python();

	}


	Py::~Py() {

		// close python
		this->close_python();
	}


	icap::Response * Py::modify( icap::Request * request ) throw() {

		/*
		*  TODO notes:
		*    + pass icap::Request to python using PyCapsule
		*      so it can be used to grab the data needed using the 'bitz'
		*      python module.
		*    + process data and use 'bitz' python module to create a icap::Response
		*      and pass it back to this method (using PyCapsule again)
		*/

		return new icap::Response( icap::ResponseHeader::NOT_IMPLEMENTED );
	}


	icap::Response * Py::preview( icap::Request * request ) throw() {
		// TODO: needs to implement, for the moment we send back
		// 100 - continue always
		return new icap::Response( icap::ResponseHeader::CONTINUE );
	}


	void Py::init_python() throw() {

		// python core
		Py_Initialize();

		// bitz python module
		Py_InitModule( "bitz", bitz_methods );

		// setup python environment
		PyObject * sys_path = PySys_GetObject( "path" );
		//PyList_Append( sys_path, )


	}


	void Py::close_python() throw() {

		// TODO: clear python stuff
		Py_Finalize();

	}

} /* end of namespace bitz */


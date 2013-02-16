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

#include <bitz/config.h>
#include <bitz/logger.h>


namespace bitz {

	Py::Py() : Modifier() {

		// defaults
		_pymodule = NULL;

		// initialise Py::config_t values
		_config.module_name = "modpy";
		_config.module_path = "";

		// load configs
		load_configs();

		// initialise python
		init_python();

	}


	Py::~Py() {

		// cleanup python
		cleanup_python();

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

		icap::Response * response;

		PyObject * pymethod;
		PyObject * pyreturn;
		PyObject * pyargs;
		PyObject * pyrequest, * pyresponse;

		// logger
		Logger &logger = Logger::instance();

		if ( _pymodule == NULL ) {
			response = new icap::Response( icap::ResponseHeader::SERVER_ERROR );
		} else {

			// call modify() in the interface module
			pymethod  = PyObject_GetAttrString( _pymodule, "modify" );
			pyargs    = PyTuple_New( 1 );
			pyrequest = PyCapsule_New( (void *) request, NULL, NULL );
			PyTuple_SetItem( pyargs, 0, pyrequest );

			if ( pymethod && PyCallable_Check( pymethod ) ) {
				pyreturn = PyObject_CallObject( pymethod, pyargs );
				Py_DECREF( pyargs );
				Py_DECREF( pyreturn );
			} else {
				logger.warn ( "[modpy] failed to call modify() in interface module" );
			}

			Py_DECREF( pymethod );

			response = new icap::Response( icap::ResponseHeader::NOT_IMPLEMENTED );
		}

		return response;

	}


	icap::Response * Py::preview( icap::Request * request ) throw() {
		// TODO: needs to implement, for the moment we send back
		// 100 - continue always
		return new icap::Response( icap::ResponseHeader::CONTINUE );
	}


	void Py::load_configs() throw() {

		std::string s;
		Config &server_config = Config::instance();

		// interface module name
		s = server_config.module_config( "modpy", "module_name" );
		if ( s != "" ) {
			_config.module_name = s;
		}

		// module lookup path
		_config.module_path = server_config.module_config( "modpy", "module_path" );

	}


	void Py::init_python() throw() {

		PyObject * sys_path;
		PyObject * pymodule_path, * pymodule_name;
		PyObject * pymethod;
		PyObject * pyreturn;

		// logger
		Logger &logger = Logger::instance();

		// python core
		Py_Initialize();

		// bitz python module
		Py_InitModule( "bitz", bitz_methods );

		// setup python environment
		if ( _config.module_path != "" ) {

			logger.debug( std::string( "[modpy] appending to sys.path, module_path: " ).append( _config.module_path ) );

			sys_path      = PySys_GetObject( "path" );
			pymodule_path = PyString_FromString( _config.module_path.c_str() );
			PyList_Append( sys_path, pymodule_path );

		}

		// load the interface module
		logger.debug( std::string( "[modpy] interface module: " ).append( _config.module_name ) );

		pymodule_name = PyString_FromString( _config.module_name.c_str() );
		_pymodule     = PyImport_Import( pymodule_name );

		if ( _pymodule != NULL ) {

			logger.debug( "[modpy] interface module loaded successfully" );

			// call init() in the interface module
			pymethod = PyObject_GetAttrString( _pymodule, "init" );

			if ( pymethod && PyCallable_Check( pymethod ) ) {
				pyreturn = PyObject_CallObject( pymethod, NULL );
				Py_DECREF( pyreturn );
			} else {
				logger.warn ( "[modpy] failed to call init() in interface module" );
			}

			Py_DECREF( pymethod );

		} else {
			logger.warn( "[modpy] failed to load interface module" );
		}


		// cleanup
		Py_DECREF( pymodule_name );
		Py_DECREF( pymodule_path );
		Py_DECREF( sys_path );

	}


	void Py::cleanup_python() throw() {

		PyObject * pymethod;
		PyObject * pyreturn;

		// logger
		Logger &logger = Logger::instance();


		// cleanup
		if ( _pymodule != NULL ) {

			// call cleanup() in the interface module
			pymethod = PyObject_GetAttrString( _pymodule, "cleanup" );

			if ( pymethod && PyCallable_Check( pymethod ) ) {
				pyreturn = PyObject_CallObject( pymethod, NULL );
				Py_DECREF( pyreturn );
			} else {
				logger.warn ( "[modpy] failed to call cleanup() in interface module" );
			}

			Py_DECREF( pymethod );
			Py_DECREF( _pymodule );

		}

		// finalise python core
		Py_Finalize();

	}

} /* end of namespace bitz */


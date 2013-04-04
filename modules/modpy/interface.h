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

#ifndef BITZ_MODPY_INTERFACE_H
#define BITZ_MODPY_INTERFACE_H

#include <Python.h>
#include <stddef.h>


PyObject * bitz_get_request( PyObject * self, PyObject * pyrequest );
PyObject * bitz_get_response( PyObject * self, PyObject * args );
PyObject * bitz_get_response_from_status( PyObject * self, PyObject * args);

static PyMethodDef bitz_methods[] = {
		{ "get_request", bitz_get_request, METH_O, "Convert a capsule request into a python dictionary" },
		{ "get_response", bitz_get_response, METH_VARARGS, "Get a response capsule (takes in status and payload dictionary as args)" },
		{ "get_response_from_status", bitz_get_response_from_status, METH_VARARGS, "Get a response capsule from a response status" },
		{ NULL, NULL, 0, NULL }
};

#endif /* !BITZ_MODPY_INTERFACE_H */


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


PyObject * bitz_get_request( PyObject * self, PyObject * args );

static PyMethodDef bitz_methods[] = {
		{ "get_request", bitz_get_request, METH_VARARGS, "Convert a capsule request into a python dictionary" },
		{ NULL, NULL, 0, NULL }
};

#endif /* !BITZ_MODPY_INTERFACE_H */


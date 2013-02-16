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


static PyMethodDef bitz_methods[] = {

};

//static int numargs=0;

/* Return the number of arguments of the application command line */
//static PyObject*
//emb_numargs(PyObject *self, PyObject *args)
//{
//    if(!PyArg_ParseTuple(args, ":numargs"))
//        return NULL;
//    return Py_BuildValue("i", numargs);
//}
//
//static PyMethodDef EmbMethods[] = {
//    {"numargs", emb_numargs, METH_VARARGS,
//     "Return the number of arguments received by the process."},
//    {NULL, NULL, 0, NULL}
//};

#endif /* !BITZ_MODPY_INTERFACE_H */


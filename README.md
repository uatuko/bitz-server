bitz-server
===========

An ICAP server implementation in C++

### Compiling from source

First you need to initialise the autotools

	$ libtoolize (glibtoolize in OS X)
	$ aclocal
	$ autoheader
	$ autoconf
	$ automake --add-missing --foreign

After that you can use the usual *./configure* && *make*


#### Notes

Use the following to create the binaries with debug symbols

	$ ../configure CPPFLAGS=-DDEBUG CXXFLAGS="-g -O0"


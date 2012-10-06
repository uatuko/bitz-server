socket-library
==============

A simple C++ socket library

#### Dependancies

- OpenSSL (>=1.0.1c)


### Compiling

First you need to initialise the autotools

	$ libtoolize (glibtoolize in OS X)
	$ aclocal
	$ autoheader
	$ autoconf
	$ automake --add-missing --foreign

After that you can use ./configure && make


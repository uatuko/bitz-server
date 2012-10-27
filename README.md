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

After that you can use the usual `./configure && make`


#### Notes
##### Debugging

Use the following to create the binaries with debug symbols

	$ ./configure CXXFLAGS="-g -O0"


##### OpenSSL (64 bit)

If you compile OpenSSL (v1.0.1) on 64bit systems you might have to compile
with `-fPIC` option. If you do that when configuring bitz-server pass in
`LIBS=-ldl` to avoid linking errors.

e.g.

	$ ./configure LIBS=-ldl


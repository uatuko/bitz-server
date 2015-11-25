bitz-server
===========

[![Build Status](https://travis-ci.org/uditha-atukorala/bitz-server.svg)](https://travis-ci.org/uditha-atukorala/bitz-server)

An ICAP server implementation in C++


## Copyright and License

Copyright (C) 2012-2015 Uditha Atukorala.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
[GNU General Public License](http://gnu.org/licenses/gpl.html)
for more details.


## The concept

The main goal of this project is to create an ICAP server
([RFC 3507](http://www.ietf.org/rfc/rfc3507.txt)) implementation in C++
to use the power of object oriented programming.

Starting from scratch, the server is developed with a modular architecture in mind.
The server core (written in C++) will handle the client requests, manage workers
(child processes) etc. and will provide basic handlers to serve ICAP requests.

To extend this core functionality the idea is to have pluggable modules (like apache
server modules). These modules will provide features like content filtering, anti-virus
scanning etc. and to make it easier to write (and faster to implement) such modules
there is hope to exploit python programming language.


## Acknowledgements

* Many thanks to Kenneth Oksanen for his support in finding and fixing bugs


## Downloads

You can download the source distributions from https://github.com/uditha-atukorala/bitz-server/releases.


## Dependencies

* [psocksxx >= 0.0.6](https://nukedzn.github.io/psocksxx/)
* [libconfig++ >= 1.4](http://www.hyperrealm.com/libconfig/)
* log4cpp >= 1.0
* python 2.7 (for modpy module)


## Bugs and Feature Requests

Please report all bugs and feature requests under [issues](https://github.com/uditha-atukorala/bitz-server/issues).


## Compiling from source

If you are using the github source then first you need to initialise autotools.

	$ libtoolize (glibtoolize in OS X)
	$ aclocal
	$ autoheader
	$ autoconf
	$ automake --add-missing

After that you can use the usual `./configure && make`


--------------------------------------------------------------------------------

### Notes
#### modpy
This is the (long awaited) python interface module. It provides a template for any
other python interface module implementations either as C++ module template or as
a C++ interface for python modules.


#### Debugging

Use the following to create the binaries with debug symbols

	$ ./configure CXXFLAGS="-g -O0"


#### config file

The default config file location is `/etc/bitz/bitz-server.conf` but this can
changed using the `--with-config` option when you run `configure`.

e.g.

	$ ./configure --with-config=/[path to source code]/conf/bitz-server.conf


#### valgrind checks

	$ valgrind --leak-check=full --read-var-info=yes --trace-children=yes --suppressions=test/valgrind.supp --log-file=valgrind.log ./src/bitz-server --debug


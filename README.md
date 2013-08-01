bitz-server
===========

An ICAP server implementation in C++

### Copyright and License

Copyright (C) 2012 Uditha Atukorala.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
[GNU General Public License](http://gnu.org/licenses/gpl.html)
for more details.


### The concept

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


### Dependencies

* [libconfig++ >= 1.4](http://www.hyperrealm.com/libconfig/)
* log4cpp >= 1.0
* python 2.7 (for modpy module)


### Bugs and Feature Requests

Please report all bugs and feature requests [here](http://bugs.geniusse.com/) under the
[bitz-server](http://bugs.geniusse.com/enter_bug.cgi?product=bitz-server) project.
Known issues can be found [here](http://bugs.geniusse.com/buglist.cgi?query_format=specific&order=relevance%20desc&bug_status=__open__&product=bitz-server&list_id=36).


### Version history

__0.1.4__ - _01st August 2013_
*   Code optimisations
*   Fixing bug [#34](http://bugs.geniusse.com/show_bug.cgi?id=34)

__0.1.3__ - _29th July 2013_
*   Fixing bugs
	[#31](http://bugs.geniusse.com/show_bug.cgi?id=31),
	[#32](http://bugs.geniusse.com/show_bug.cgi?id=32)

__0.1.2__ - _07th July 2013_
*   Added a RESPMOD handler ([bug #16](http://bugs.geniusse.com/show_bug.cgi?id=16))
*   Added Message Preview features ([bug #17](http://bugs.geniusse.com/show_bug.cgi?id=17))
*   Code cleanup and optimisations
*   Fixing bugs [#15](http://bugs.geniusse.com/show_bug.cgi?id=15),
	[#23](http://bugs.geniusse.com/show_bug.cgi?id=23),
	[#24](http://bugs.geniusse.com/show_bug.cgi?id=24),
	[#25](http://bugs.geniusse.com/show_bug.cgi?id=25)

__0.1.1__ - _06th March 2013_
*   Fixing bugs [#19](http://bugs.geniusse.com/show_bug.cgi?id=19) and [#22](http://bugs.geniusse.com/show_bug.cgi?id=22)

__0.1.0__ - _03rd March 2013_

*   Daemonized version ([bug #18](http://bugs.geniusse.com/show_bug.cgi?id=18)), hence
	the minor version bump. Server core is re-organised with bitz::server namespace to
	be more cleaner and easier to read.
*   Make it possible to pass in command-line options
*   Closed a memory leak in modpy module
*   Fixing bugs [#20](http://bugs.geniusse.com/show_bug.cgi?id=20) and [#21](http://bugs.geniusse.com/show_bug.cgi?id=21)


__0.0.1__ - _24th February 2013_

*   Proof of concept. An ICAP server with only a REQMOD handler. Includes a template
	_echo_ module to demonstrate the pluggable module architecture and the _modpy_ module
	to demonstrate the python interface.

---------------------------------------


### Compiling from source

First you need to initialise the autotools

	$ libtoolize (glibtoolize in OS X)
	$ aclocal
	$ autoheader
	$ autoconf
	$ automake --add-missing --foreign

After that you can use the usual `./configure && make`


#### Notes
##### modpy
This is the (long awaited) python interface module. It provides a template for any
other python interface module implementations either as C++ module template or as
a C++ interface for python modules.


##### Debugging

Use the following to create the binaries with debug symbols

	$ ./configure CXXFLAGS="-g -O0"


##### OpenSSL (64 bit)

If you compile OpenSSL (v1.0.1) on 64bit systems you might have to compile
with `-fPIC` option. If you do that when configuring bitz-server pass in
`LIBS=-ldl` to avoid linking errors.

e.g.

	$ ./configure LIBS=-ldl


##### config file

The default config file location is `/etc/bitz/bitz-server.conf` but this can
changed using the `--with-config` option when you run `configure`.

e.g.

	$ ./configure --with-config=/[path to source code]/conf/bitz-server.conf


##### valgrind checks

	$ valgrind --leak-check=full --read-var-info=yes --trace-children=yes --suppressions=test/valgrind.supp --log-file=valgrind.log ./src/bitz-server --debug


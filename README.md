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



How to use
----------

This library is not supposed to be installed and there is no install
directive. Rather you should use this in your own projects.

e.g. [bitz-server](https://github.com/uditha-atukorala/bitz-server)

If you want to use this in your own git repository and decided to use subtree
merging technique, following should help you.

```bash
$ git remote add -f socket-library https://github.com/uditha-atukorala/socket-library.git
$ git merge -s ours --no-commit socket-library/master

$ git read-tree --prefix=lib/socket -u socket-library/master:lib
$ git commit
```

When you want to pull changes from the socket-library and update your source
you can use the following.

```bash
$ git pull -s subtree socket-library master
```


# doc/

# man
man1_MANS = bitz-server.man
man5_MANS = bitz-server.conf.man

# extra dist files
EXTRA_DIST = \
	bitz-server.man \
	bitz-server.conf.man


# doxygen
if HAVE_DOXYGEN

.PHONY: doxygen-doc

doxygen.stamp:
	$(DOXYGEN) doxygen.cfg
	echo timestamp > doxygen.stamp

CLEANFILES = doxygen.stamp

doxygen-doc: doxygen.stamp

clean-local:
	rm -rf $(top_builddir)/doc/doxygen

endif


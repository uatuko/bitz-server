## conf/
configdir		= $(sysconfdir)/bitz
config_DATA		= bitz-server.conf

# extra dist files
EXTRA_DIST = \
	bitz-server.conf.in


edit = sed \
	-e 's|@sysconfdir[@]|$(sysconfdir)|g' \
	-e 's|@localstatedir[@]|$(localstatedir)|g' \
	-e 's|@pkglibdir[@]|$(pkglibdir)|g'

bitz-server.conf: bitz-server.conf.in
	rm -f $@ $@.tmp
	srcdir=''; \
		test -f ./$@.in || srcdir=$(srcdir)/; \
		$(edit) $${srcdir}$@.in >$@.tmp
	chmod +r $@.tmp
	chmod og-wx $@.tmp
	mv $@.tmp $@

clean-local:
	rm -f *.conf


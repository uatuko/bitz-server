## conf/
configdir		= $(sysconfdir)/bitz
config_DATA		= bitz-server.conf

logdir          = $(localstatedir)/log/bitz
log_DATA        = bitz-server.log


edit = sed \
	-e 's|@sysconfdir[@]|$(sysconfdir)|g' \
	-e 's|@localstatedir[@]|$(localstatedir)|g'

bitz-server.log:
	touch $@

bitz-server.conf: Makefile
	rm -f $@ $@.tmp
	srcdir=''; \
		test -f ./$@.in || srcdir=$(srcdir)/; \
		$(edit) $${srcdir}$@.in >$@.tmp
	chmod +r $@.tmp
	chmod og-wx $@.tmp
	mv $@.tmp $@

clean-local:
	rm -f *.conf
	rm -f *.log


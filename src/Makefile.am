## Process this file with automake to produce Makefile.in

bin_PROGRAMS = wmcoincoin

EXTRA_wmcoincoin_SOURCES = http_win.c http_unix.c

wmcoincoin_SOURCES = \
	myprintf.c myprintf.h defaultrcfile.h defaultoptionsfile.h \
	raster.c raster.h troll_detector.c troll_data.h \
	pinnipede.c editwin.c newswin.c \
	picohtml.c picohtml.h msgbox.c balloon.c \
	wmcoincoin.c coincoin.h global.h http.h \
	coin_util.c coin_util.h \
	coincoin_prefs.c coincoin_prefs.h \
	coincoin_news.c coincoin_tribune.c \
	regexp.h regexp.c general.h \
	spell_coin.c spell_coin.h

wmcoincoin_LDADD = http.o

http.c: @HTTP_SRC@
	ln -sf @HTTP_SRC@ http.c

install: install-am
# petite update merci anvil !
#	ln -sf $(DESTDIR)$(bindir)/wmcoincoin $(DESTDIR)$(bindir)/wmcoincoin-kde
	(cd $(DESTDIR)$(bindir) ; ln -sf wmcoincoin wmcoincoin-kde)

## rcsid=$Id: Makefile.am,v 1.5 2002/01/18 00:28:42 pouaite Exp $
## ChangeLog:
##   $Log: Makefile.am,v $
##   Revision 1.5  2002/01/18 00:28:42  pouaite
##   le ménage continue + grosses modifs (experimentales pour l'instant)
##
##   Revision 1.4  2002/01/16 21:27:35  pouaite
##   gros coup de balai dans wmcoincoin.c qui s'est du coup splitté en trois: wmcoincoin.c, dock.c et useragents_file.c
##
##   Revision 1.3  2002/01/13 15:19:00  pouaite
##   double patch: shift -> tribune.post_cmd et lordOric -> tribune.archive
##
##   Revision 1.2  2001/12/02 18:32:05  pouaite
##   update suite à la modif sur http.c et a la generation automatique de src/default*.h
##
## Process this file with automake to produce Makefile.in

bin_PROGRAMS = wmcoincoin

EXTRA_wmcoincoin_SOURCES = http_win.c http_unix.c  # include dans http.c

wmcoincoin_SOURCES = \
	myprintf.c myprintf.h defaultuseragents.h defaultoptionsfile.h \
	raster.c raster.h troll_detector.c troll_data.h \
	dock.c useragents_file.c tribune_util.c \
	pinnipede.c editwin.c newswin.c \
	picohtml.c picohtml.h msgbox.c balloon.c \
	wmcoincoin.c coincoin.h global.h http.c http.h \
	coin_util.c coin_util.h \
	coincoin_prefs.c coincoin_prefs.h \
	coincoin_news.c coincoin_tribune.c \
	regexp.h regexp.c general.h \
	spell_coin.c spell_coin.h

defaultuseragents.h: ../useragents
	cat ../useragents | sed -e '/^#%.*/d' | sed -e 's/\\/\\\\/g' | sed -e 's/	/\\t/g' | sed -e 's/^/fprintf(f,"/g' | sed -e 's/$$/\\n");/g' > ./defaultuseragents.h

defaultoptionsfile.h: ../options
	cat ../options | sed -e '/^#%.*/d' | sed -e 's/\\/\\\\/g' | sed -e 's/"/\\"/g' | sed -e 's/	/\\t/g' | sed -e 's/^/fprintf(f,"/g' | sed -e 's/$$/\\n");/g' | sed -e 's/%/%%/g' > ./defaultoptionsfile.h

wmcoincoin.o: defaultuseragents.h

coincoin_prefs.o: defaultoptionsfile.h

install: install-am
# petite update merci anvil !
#	ln -sf $(DESTDIR)$(bindir)/wmcoincoin $(DESTDIR)$(bindir)/wmcoincoin-kde
	(cd $(DESTDIR)$(bindir) ; ln -sf wmcoincoin wmcoincoin-kde)

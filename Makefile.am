## Process this file with automake to produce Makefile.in

SUBDIRS = src doc
EXTRA_DIST = xpms/porte.h xpms/editwin_minib.xcf xpms/editwin_minib.xpm xpms/coin.xpm  xpms/leds.h  xpms/fleche.xpm xpms/leds.xcf xpms/porte.xcf xpms/pp_minib.xpm xpms/weekday.xpm xpms/month.xpm xpms/clock.xpm xpms/date.xpm xpms/led.xpm ./wmcoincoin.spec ./wmpanpan ./config.sub ./config.guess

rpm-dist: dist
	cp $(PACKAGE)-$(VERSION).tar.gz $(HOME)/rpm/SOURCES
	-rm -f $(HOME)/rpm/SOURCES/$(PACKAGE)-$(VERSION).tar.bz2
	bzme $(HOME)/rpm/SOURCES/$(PACKAGE)-$(VERSION).tar.gz
	rpm -ba $(PACKAGE).spec

bin-dist: all
	cp src/wmcoincoin .
	strip wmcoincoin
	tar czvf $(PACKAGE)-$(VERSION)-$(COIN_ARCH).tgz README wmcoincoin

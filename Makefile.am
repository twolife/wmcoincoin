## rcsid=$Id: Makefile.am,v 1.12 2002/06/09 11:42:46 pouaite Exp $
## ChangeLog:
##   $Log: Makefile.am,v $
##   Revision 1.12  2002/06/09 11:42:46  pouaite
##   patch lordOric
##
##   Revision 1.11  2002/06/01 20:21:18  pouaite
##   version 2.3.8a
##
##   Revision 1.10  2002/06/01 17:54:04  pouaite
##   nettoyage
##
##   Revision 1.9  2002/05/29 22:38:12  pouaite
##   bidouilles dans configure.in et cie
##
##   Revision 1.8  2002/05/25 10:53:05  pouaite
##   aujourd'hui c'est la saint beugue
##
##   Revision 1.7  2002/04/15 19:56:38  pouaite
##   v2.3.7a
##
##   Revision 1.6  2002/04/13 11:55:19  pouaite
##   fix kde3 + deux trois conneries
##
##   Revision 1.5  2002/03/05 21:04:28  pouaite
##   bugfixes suite à l'upgrade de dlfp [et retour au comportement à l'ancienne du clic sur les horloges pour les moules ronchonnes]
##
##   Revision 1.4  2001/12/16 16:46:12  pouaite
##   Clippouille joins C0IN C0IN
##
##   Revision 1.3  2001/12/02 20:29:31  pouaite
##   rajout de 'options' et 'useragents' dans le Makefile.am ...
##
##   Revision 1.2  2001/12/02 18:32:05  pouaite
##   update suite à la modif sur http.c et a la generation automatique de src/default*.h
##
## Process this file with automake to produce Makefile.in

#AUTOMAKE_OPTIONS = 1.5 dist-bzip2

# pour faire plaisir à woof :°)
prefix=$(DESTDIR)@prefix@

SUBDIRS = src doc
EXTRA_DIST = xpms/porte.h xpms/editwin_minib.xcf xpms/editwin_minib.xpm \
             xpms/coin.xpm  xpms/leds.h  xpms/leds.xcf \
             xpms/porte.xcf  xpms/weekday.xpm xpms/month.xpm \
             xpms/clock.xpm xpms/date.xpm xpms/led.xpm xpms/clippy.xpm xpms/icon.xpm \
             ./wmcoincoin.spec ./wmpanpan ./config.sub ./config.guess \
             ./useragents ./options

rpm-dist: dist
	cp $(PACKAGE)-$(VERSION).tar.gz $(HOME)/rpm/SOURCES
	-rm -f $(HOME)/rpm/SOURCES/$(PACKAGE)-$(VERSION).tar.bz2
	bzme $(HOME)/rpm/SOURCES/$(PACKAGE)-$(VERSION).tar.gz
	rpm -ba $(PACKAGE).spec

bin-dist: all
	cp src/wmcoincoin .
	strip wmcoincoin
	tar czvf $(PACKAGE)-$(VERSION)-$(COIN_ARCH).tgz ChangeLog README options useragents wmcoincoin

pkgdata_DATA = options useragents README wmcoincoin.spec

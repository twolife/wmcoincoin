#!/bin/sh
# Run this to generate all the initial makefiles, etc.

DIE=0

echo "Generating configuration files for wmcoincoin, please wait...."

echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING "
echo "you need autoconf-2.5x AND automake-1.6 to generate the makefiles ..."
echo "please check that autoconf --version give 2.5x and automake --version gives 1.6"
echo "you will also need a recent gettext (0.11.5 is fine)"
echo "à la main, le 0.10.40 marche aussi , mais pas 0.10.35"
echo 
echo 
echo 
echo 
echo 
echo 
echo "yes it sucks"
echo

(autoconf --version) < /dev/null > /dev/null 2>&1 || {
	echo
        echo "You must have autoconf installed to compile wmcoincoin."
        echo "Download the appropriate package for your distribution,"
        echo "or get the source tarball at ftp://ftp.gnu.org/pub/gnu/"
        DIE=1
}

(automake --version) < /dev/null > /dev/null 2>&1 || {
        echo
        echo "You must have automake installed to compile wmcoincoin."
        echo "Download the appropriate package for your distribution,"
        echo "or get the source tarball at ftp://ftp.gnu.org/pub/gnu/"
        DIE=1
}

(gettextize --version) < /dev/null > /dev/null 2>&1 || {
        echo
        echo "You must have gettext and gettext-devel installed to compile wmcoincoin from cvs."
        DIE=1
}

if test "$DIE" -eq 1; then
        exit 1
fi

GETTEXT_VERSION=`gettextize --version | sed -n 's/^.*\([0-9]\+\)\.\([0-9]\+\)\.\([0-9]\+\).*$/\1.\2.\3/p'`
GETTEXT_MAJOR_VERSION=`echo $GETTEXT_VERSION | sed -n 's/^\([0-9]\+\).*/\1/p'`
GETTEXT_MINOR_VERSION=`echo $GETTEXT_VERSION | sed -n 's/^[0-9]\+\.\([0-9]\+\).*/\1/p'`
GETTEXT_MICRO_VERSION=`echo $GETTEXT_VERSION | sed -n 's/^[0-9]\+\.[0-9]\+\.\([0-9]\+\).*/\1/p'`

if [ $GETTEXT_MINOR_VERSION -eq 11 ]; then
    #works at least with gettext-0.11.5
    rm -f ChangeLog~ ; rm -f configure.ac~; rm -f Makefile.am~
    echo "  gettextize --copy --force --intl"
    gettextize --copy --force --intl || exit
    echo "cleaning the crap of gettextize.."
    [ -e ChangeLog~ ] && mv -f ChangeLog~ ChangeLog
    [ -e configure.ac~ ] && mv -f configure.ac~ configure.ac
    [ -e Makefile.am~ ] && mv -f Makefile.am~ Makefile.am
else
    #works at least with gettext-0.10.40
    # ... except the link ...
    echo "  gettextize --copy --force"
    gettextize --copy --force || exit;
    mkdir -p m4
    GTM4=`which gettext | sed -e 's%bin/gettext%share/aclocal%'`;
    echo gtm4 $GTM4
    for i in codeset.m4 gettext.m4 glibc21.m4 iconv.m4 isc-posix.m4 lcmessage.m4  progtest.m4; do
      cp -f $GTM4/$i ./m4
    done
    cp doc/Makefile.am m4/Makefile.am
    echo "****************************************"
    echo " il faudra surement virer @LIBINTL@ de src/Makefile.am !!"
    echo "****************************************"
fi;

#cp po/Makefile.in.in intl/Makefile.in.in
echo "  aclocal -I m4 $ACLOCAL_FLAGS"
aclocal -I m4 $ACLOCAL_FLAGS
echo "  autoheader"
autoheader || exit;
echo "  automake --add-missing --gnu --include-deps"
automake --add-missing --gnu --include-deps
echo "  autoconf"
autoconf || exit;

if test -z "$*"; then
    echo "I am going to run ./configure with no arguments - if you wish "
    echo "to pass any to it, please specify them on the $0 command line."
    echo "If you do not wish to run ./configure, press Ctrl-C now."
    trap 'echo "configure aborted" ; exit 0' 1 2 15
    sleep 1
fi
./configure "$@"

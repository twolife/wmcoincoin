#!/bin/sh
# Run this to generate all the initial makefiles, etc.

DIE=0

echo "Generating configuration files for wmcoincoin, please wait...."

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

echo "  aclocal -I . $ACLOCAL_FLAGS"
aclocal -I m4 $ACLOCAL_FLAGS
echo "  gettextize --copy --force --intl"
gettextize --copy --force --intl
echo "  autoheader"
autoheader
echo "  automake --add-missing --gnu --include-deps"
automake --add-missing --gnu --include-deps
echo "  autoconf"
autoconf

if test -z "$*"; then
    echo "I am going to run ./configure with no arguments - if you wish "
    echo "to pass any to it, please specify them on the $0 command line."
    echo "If you do not wish to run ./configure, press Ctrl-C now."
    trap 'echo "configure aborted" ; exit 0' 1 2 15
    sleep 1
fi
./configure "$@"

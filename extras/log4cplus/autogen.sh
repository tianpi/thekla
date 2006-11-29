#!/bin/sh

libtoolize --force --automake --copy

autoheader
aclocal $ACLOCAL_FLAGS
autoconf
automake --ignore-deps --add-missing --copy

echo 'run "configure; make"'


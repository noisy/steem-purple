#!/bin/sh

echo "Generating ./configure file ..."

aclocal || exit;
autoheader || exit;
autoconf || exit;

echo;
echo "Done."

dnl $Id$
dnl config.m4 for extension to_utf8

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(to_utf8, for to_utf8 support,
dnl Make sure that the comment is aligned:
dnl [  --with-to_utf8             Include to_utf8 support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(to_utf8, whether to enable to_utf8 support,
dnl Make sure that the comment is aligned:
[  --enable-to_utf8           Enable to_utf8 support])

if test "$PHP_TO_UTF8" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-to_utf8 -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/to_utf8.h"  # you most likely want to change this
  dnl if test -r $PHP_TO_UTF8/$SEARCH_FOR; then # path given as parameter
  dnl   TO_UTF8_DIR=$PHP_TO_UTF8
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for to_utf8 files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       TO_UTF8_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$TO_UTF8_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the to_utf8 distribution])
  dnl fi

  dnl # --with-to_utf8 -> add include path
  dnl PHP_ADD_INCLUDE($TO_UTF8_DIR/include)

  dnl # --with-to_utf8 -> check for lib and symbol presence
  dnl LIBNAME=to_utf8 # you may want to change this
  dnl LIBSYMBOL=to_utf8 # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $TO_UTF8_DIR/lib, TO_UTF8_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_TO_UTF8LIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong to_utf8 lib version or lib not found])
  dnl ],[
  dnl   -L$TO_UTF8_DIR/lib -lm -ldl
  dnl ])
  dnl
  dnl PHP_SUBST(TO_UTF8_SHARED_LIBADD)

  PHP_REQUIRE_CXX()
  PHP_ADD_LIBRARY(stdc++, "", EXTRA_LDFLAGS)
  PHP_NEW_EXTENSION(to_utf8, to_utf8.cpp, $ext_shared)
fi

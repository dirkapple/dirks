dnl $Id$
dnl config.m4 for extension cntwd

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(cntwd, for cntwd support,
dnl [  --with-cntwd             Include cntwd support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(cntwd, whether to enable cntwd support,
[  --enable-cntwd           Enable cntwd support])

if test "$PHP_CNTWD" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-cntwd -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/cntwd.h"  # you most likely want to change this
  dnl if test -r $PHP_CNTWD/$SEARCH_FOR; then # path given as parameter
  dnl   CNTWD_DIR=$PHP_CNTWD
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for cntwd files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       CNTWD_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$CNTWD_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the cntwd distribution])
  dnl fi

  dnl # --with-cntwd -> add include path
  dnl PHP_ADD_INCLUDE($CNTWD_DIR/include)

  dnl # --with-cntwd -> check for lib and symbol presence
  dnl LIBNAME=cntwd # you may want to change this
  dnl LIBSYMBOL=cntwd # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $CNTWD_DIR/lib, CNTWD_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_CNTWDLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong cntwd lib version or lib not found])
  dnl ],[
  dnl   -L$CNTWD_DIR/lib -lm -ldl
  dnl ])
  dnl
  dnl PHP_SUBST(CNTWD_SHARED_LIBADD)

  PHP_REQUIRE_CXX()
  PHP_ADD_LIBRARY(stdc++, "", EXTRA_LDFLAGS)
  PHP_NEW_EXTENSION(cntwd, cntwd.cpp, $ext_shared)
fi

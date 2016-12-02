dnl $Id$
dnl config.m4 for extension psproto

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(psproto, for psproto support,
dnl Make sure that the comment is aligned:
dnl [  --with-psproto             Include psproto support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(psproto, whether to enable psproto support,
dnl Make sure that the comment is aligned:
dnl [  --enable-psproto           Enable psproto support])

if test "$PHP_PSPROTO" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-psproto -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/psproto.h"  # you most likely want to change this
  dnl if test -r $PHP_PSPROTO/$SEARCH_FOR; then # path given as parameter
  dnl   PSPROTO_DIR=$PHP_PSPROTO
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for psproto files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       PSPROTO_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$PSPROTO_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the psproto distribution])
  dnl fi

  dnl # --with-psproto -> add include path
  dnl PHP_ADD_INCLUDE($PSPROTO_DIR/include)

  dnl # --with-psproto -> check for lib and symbol presence
  dnl LIBNAME=psproto # you may want to change this
  dnl LIBSYMBOL=psproto # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $PSPROTO_DIR/$PHP_LIBDIR, PSPROTO_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_PSPROTOLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong psproto lib version or lib not found])
  dnl ],[
  dnl   -L$PSPROTO_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(PSPROTO_SHARED_LIBADD)

  PHP_NEW_EXTENSION(psproto, psproto.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi

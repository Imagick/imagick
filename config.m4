dnl $Id$
dnl config.m4 for extension imagick

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(imagick, for imagick support,
Make sure that the comment is aligned:
[  --with-imagick             Include imagick support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(imagick, whether to enable imagick support,
dnl Make sure that the comment is aligned:
dnl [  --enable-imagick           Enable imagick support])

if test "$PHP_IMAGICK" != "no"; then
  dnl Write more examples of tests here...
  PHP_ADD_LIBRARY(Magick)
  dnl # --with-imagick -> check with-path
	dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/magick.h"  # you most likely want to change this
  dnl if test -r $PHP_imagick/$SEARCH_FOR; then # path given as parameter
  dnl   imagick_DIR=$PHP_imagick
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for imagick files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       imagick_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$imagick_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the imagick distribution])
  dnl fi

  dnl # --with-imagick -> add include path
  dnl PHP_ADD_INCLUDE($imagick_DIR/include)

  dnl # --with-imagick -> chech for lib and symbol presence
  dnl LIBNAME=imagick # you may want to change this
  dnl LIBSYMBOL=imagick # you most likely want to change this 
  dnl old_LIBS=$LIBS
  dnl LIBS="$LIBS -L$imagick_DIR/lib -lm -ldl"
  dnl AC_CHECK_LIB($LIBNAME, $LIBSYMBOL, [AC_DEFINE(HAVE_imagick_LIB,1,[ ])],
	dnl			[AC_MSG_ERROR([wrong imagick lib version or lib not found])])
  dnl LIBS=$old_LIBS
  dnl
  dnl PHP_SUBST(imagick_SHARED_LIBADD)
  dnl PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $imagick_DIR/lib, imagick_SHARED_LIBADD)

  PHP_EXTENSION(imagick, $ext_shared)
fi

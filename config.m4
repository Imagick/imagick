dnl
dnl $Id$
dnl

PHP_ARG_WITH(imagick, for imagick support,
Make sure that the comment is aligned:
[  --with-imagick[=DIR]       Include imagick support])

if test "$PHP_IMAGICK" != "no"; then
  if test -r $PHP_IMAGICK/include/magick/magick.h; then
    IMAGEMAGICK_DIR=$PHP_IMAGICK
  else
    AC_MSG_CHECKING(for ImageMagick in default path)
    for i in /usr/local /usr; do
      if test -r $i/include/magick/magick.h; then
        IMAGEMAGICK_DIR=$i
        AC_MSG_RESULT(found in $i)
      fi
    done
  fi

  if test -z "$IMAGEMAGICK_DIR"; then
    AC_MSG_RESULT(not found)
    AC_MSG_ERROR(Please reinstall the ImageMagick distribution -
    magick.h should be in <imagemagick-dir>/include/magick/)
  fi

  IMAGEMAGICK_CONFIG="Magick-config"
  AC_MSG_CHECKING(for ImageMagick 5.3.8 or greater)

  if ${IMAGEMAGICK_DIR}/bin/Magick-config --libs print > /dev/null 2>&1; then
    IMAGEMAGICK_CONFIG=${IMAGEMAGICK_DIR}/bin/Magick-config
  else
    if ${IMAGEMAGICK_DIR}/Magick-config --libs print > /dev/null 2>&1; then
       IMAGEMAGICK_CONFIG=${IMAGEMAGICK_DIR}/Magick-config
    fi
  fi
  
  imagemagick_version_full=`$IMAGEMAGICK_CONFIG --version`
  imagemagick_version=`echo ${imagemagick_version_full} | awk 'BEGIN { FS = "."; } { printf "%d", ($1 * 1000 + $2) * 1000 + $3;}'`
  AC_MSG_RESULT($imagemagick_version_full)
  if test "$imagemagick_version" -ge 5003008; then
    IMAGEMAGICK_LIBS=`$IMAGEMAGICK_CONFIG --libs`
    IMAGEMAGICK_LDFLAGS=`$IMAGEMAGICK_CONFIG --ldflags`    
  else
    AC_MSG_ERROR(ImageMagick version 5.3.8 or later is required to compile php with imagick support)
  fi

  PHP_ADD_INCLUDE($IMAGEMAGICK_DIR/include)
  PHP_EVAL_LIBLINE($IMAGEMAGICK_LIBS, IMAGICK_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(Magick, $IMAGEMAGICK_DIR/lib, IMAGICK_SHARED_LIBADD)

  AC_CHECK_LIB(Magick,InitializeMagick, 
  [ 
    AC_DEFINE(HAVE_IMAGICK,1,[ ])
  ],[
    AC_MSG_ERROR(There is something wrong. Please check config.log for more information.)
  ],[
    $IMAGEMAGICK_LIBS -L$IMAGEMAGICK_DIR/lib $IMAGEMAGICK_LDFLAGS
  ])

  PHP_EXTENSION(imagick, $ext_shared)
  PHP_SUBST(IMAGICK_SHARED_LIBADD)
fi

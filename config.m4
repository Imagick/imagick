dnl
dnl $Id$
dnl

PHP_ARG_WITH(imagick, for imagick support,
[  --with-imagick[=DIR]    Include imagick support], no)
PHP_ARG_WITH(imagick-gm, for imagick support,
[  --with-imagick-gm       Imagick: Use GraphicsMagick instead of ImageMagick as Backend], no, no)



if test "$PHP_IMAGICK" != "no"; then
  if test "$PHP_IMAGICK_GM" != "no"; then
     GM_PATH="GraphicsMagick/"
     MAGICK_NAME="GraphicsMagick"
     NEEDED_VERSION=1001000
     NEEDED_VERSION_STRING=1.1.0
     CONFIG_NAME="GraphicsMagick-config"
     LIB_NAME="GraphicsMagick"
     AC_DEFINE(IMAGICK_BACKEND, "GraphicsMagick", [ ])
     AC_DEFINE(HAVE_GRAPHICSMAGICK, 1, [ ])
     AC_MSG_RESULT(Building with GraphicsMagick as backend)
  else
     GM_PATH=""
     MAGICK_NAME="ImageMagick"
     NEEDED_VERSION=5005003
     NEEDED_VERSION_STRING=5.5.3
     CONFIG_NAME="Magick-config"
     LIB_NAME="Magick"
     AC_DEFINE(IMAGICK_BACKEND, "ImageMagick", [ ])
     AC_DEFINE(HAVE_IMAGEMAGICK, 1, [ ])
     AC_MSG_RESULT(Building with ImageMagick as backend)
  fi
  if test -r $PHP_IMAGICK/include/${GM_PATH}magick/magick.h; then
    AC_MSG_CHECKING(for $MAGICK_NAME in provided path)
    IMAGEMAGICK_DIR=$PHP_IMAGICK
    AC_MSG_RESULT(found in $IMAGEMAGICK_DIR)
  else
    AC_MSG_CHECKING(for $MAGICK_NAME in default path)
    for i in /usr/local /usr; do
      if test -r $i/include/${GM_PATH}magick/magick.h; then
        IMAGEMAGICK_DIR=$i
        AC_MSG_RESULT(found in $i)
      fi
    done

    dnl red hat has some strange locations for their imagemagick files
    dnl so here's for redhat

    if test -z "$IMAGEMAGICK_DIR"; then
      for i in /usr/local/X11R6 /usr/X11R6; do
        if test -r $i/include/X11/${GM_PATH}magick/magick.h; then
          IMAGEMAGICK_DIR=$i
          AC_MSG_RESULT(found in $i)
        fi
      done
    fi
  fi

  if test -z "$IMAGEMAGICK_DIR"; then
    AC_MSG_RESULT(not found)
    AC_MSG_ERROR(Please reinstall the $MAGICK_NAME distribution -
    magick.h should be in <imagemagick-dir>/include/magick/)
  fi
  IMAGEMAGICK_CONFIG="${CONFIG_NAME}"

  if ${IMAGEMAGICK_DIR}/bin/${CONFIG_NAME} --libs  > /dev/null 2>&1; then
    IMAGEMAGICK_CONFIG=${IMAGEMAGICK_DIR}/bin/${CONFIG_NAME}
  else
    if ${IMAGEMAGICK_DIR}/${CONFIG_NAME} --libs  > /dev/null 2>&1; then
       IMAGEMAGICK_CONFIG=${IMAGEMAGICK_DIR}/${CONFIG_NAME}
    fi
  fi
  AC_MSG_CHECKING(for ${MAGICK_NAME} ${NEEDED_VERSION_STRING} or later with $IMAGEMAGICK_CONFIG)
  imagemagick_version_full=`$IMAGEMAGICK_CONFIG --version`
  imagemagick_version=`echo ${imagemagick_version_full} | awk 'BEGIN { FS = "."; } { printf "%d", ($1 * 1000 + $2) * 1000 + $3;}'`
  AC_MSG_RESULT($imagemagick_version_full)
  if test "$imagemagick_version" -lt ${NEEDED_VERSION}; then
    AC_MSG_ERROR(${MAGICK_NAME} ${NEEDED_VERSION_STRING} or later is required to compile php with imagick support)
  else
    IMAGEMAGICK_LIBS=`$IMAGEMAGICK_CONFIG --libs`
    IMAGEMAGICK_LDFLAGS=`$IMAGEMAGICK_CONFIG --ldflags`    
  fi

  PHP_ADD_INCLUDE($IMAGEMAGICK_DIR/include/${GM_PATH})
  PHP_EVAL_LIBLINE($IMAGEMAGICK_LIBS, IMAGICK_SHARED_LIBADD)
  PHP_EVAL_LIBLINE($IMAGEMAGICK_LDFLAGS, IMAGICK_SHARED_LIBADD)

  AC_CHECK_LIB(${LIB_NAME},InitializeMagick, 
  [ 
    AC_DEFINE(HAVE_IMAGICK,1,[ ])
  ],[
    AC_MSG_ERROR(There is something wrong. Please check config.log for more information.)
  ],[
    $IMAGEMAGICK_LIBS -L$IMAGEMAGICK_DIR/lib $IMAGEMAGICK_LDFLAGS
  ])
  AC_MSG_CHECKING(if new ImageMagick API is needed)
  if test "$MAGICK_NAME" = "ImageMagick" && test "$imagemagick_version" -ge 6000000; then
    AC_MSG_RESULT(yes. Adding compatibility options)
    AC_DEFINE(IMAGICK_NEEDS_WAND,1,[ ])
    PHP_ADD_LIBRARY_WITH_PATH(Wand, $IMAGEMAGICK_DIR/lib, IMAGICK_SHARED_LIBADD)
  elif test "$MAGICK_NAME" = "GraphicsMagick" && test "$imagemagick_version" -ge 1001000; then
    AC_MSG_RESULT(yes. Adding compatibility options)
    AC_DEFINE(IMAGICK_NEEDS_WAND,1,[ ])
    PHP_ADD_LIBRARY_WITH_PATH(GraphicsMagickWand, $IMAGEMAGICK_DIR/lib, IMAGICK_SHARED_LIBADD)
  else
    AC_MSG_RESULT(no)
  fi
  PHP_EXTENSION(imagick, $ext_shared)
  PHP_SUBST(IMAGICK_SHARED_LIBADD)
fi

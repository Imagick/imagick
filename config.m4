PHP_ARG_WITH(imagick, whether to enable the imagick extension,
[ --with-imagick[=DIR]	Enables the imagick extension. DIR is the prefix to Imagemagick installation directory.], no)

PHP_ARG_WITH(imagick-gm, whether to enable the imagick GraphicsMagick backend,
[ --with-imagick-gm[=DIR]	GraphicsMagick backend. NO LONGER SUPPORTED!], no, no)

if test $PHP_IMAGICK_GM != "no"; then
	AC_MSG_ERROR(GraphicsMagick backend is no longer supported)
fi

if test $PHP_IMAGICK != "no"; then

  AC_MSG_CHECKING(ImageMagick MagickWand API configuration program)

  for i in $PHP_IMAGICK /usr/local /usr;
  do
    test -r $i/bin/MagickWand-config && IMAGICK_AFTER_BWC_BREAK=true && WAND_BINARY=$i/bin/MagickWand-config && break
  done

  if test -z "$WAND_BINARY"; then
    for i in $PHP_IMAGICK /usr/local /usr;
    do
      test -r $i/bin/Wand-config && WAND_BINARY=$i/bin/Wand-config && break
    done
  fi

  if test -z "$WAND_BINARY"; then
    AC_MSG_ERROR(not found. Please provide a path to MagickWand-config or Wand-config program.)
  fi

  AC_MSG_RESULT(found in $WAND_BINARY)

  IMAGEMAGICK_VERSION_ORIG=`$WAND_BINARY --version`
  IMAGEMAGICK_VERSION_MASK=`echo ${IMAGEMAGICK_VERSION_ORIG} | awk 'BEGIN { FS = "."; } { printf "%d", ($1 * 1000 + $2) * 1000 + $3;}'`

  AC_MSG_CHECKING(if ImageMagick version is at least 6.2.4)
  if test "$IMAGEMAGICK_VERSION_MASK" -ge 6002004; then
    AC_MSG_RESULT(found version $IMAGEMAGICK_VERSION_ORIG)
  else
    AC_MSG_ERROR(no. You need at least Imagemagick version 6.2.4 to use Imagick.)
  fi

  WAND_DIR=`$WAND_BINARY --prefix` 

  if test -z "$IMAGICK_AFTER_BWC_BREAK"; then
    AC_MSG_CHECKING(for magick-wand.h header file)
    if test -r $WAND_DIR/include/wand/magick-wand.h; then
      AC_MSG_RESULT(found in $WAND_DIR/include/wand/magick-wand.h)
    else
      AC_MSG_ERROR(Cannot locate header file magick-wand.h)
    fi	
  elif test "$IMAGEMAGICK_VERSION_MASK" -gt 6008000; then
    if test -r $WAND_DIR/include/ImageMagick-6/wand/magick-wand.h; then
      AC_MSG_RESULT(found in $WAND_DIR/include/ImageMagick-6/wand/magick-wand.h)
    else
        AC_MSG_ERROR(Cannot locate header file magick-wand.h)
    fi 
  else
    AC_MSG_CHECKING(for MagickWand.h header file)

    if test -r $WAND_DIR/include/ImageMagick/wand/MagickWand.h; then
      AC_MSG_RESULT(found in $WAND_DIR/include/ImageMagick/wand/MagickWand.h)
    else
      AC_MSG_ERROR(Cannot locate header file MagickWand.h)
    fi
    AC_DEFINE(IMAGICK_USE_NEW_HEADER,1,[ ])
    PHP_IMAGICK_USE_NEW_HEADER=1
  fi

  AC_MSG_CHECKING([PHP version is at least 5.1.3])

  if test -z "${PHP_VERSION_ID}"; then
    if test -z "${PHP_CONFIG}"; then
      AC_MSG_ERROR([php-config not found])
    fi
    if test -z "${AWK}"; then
      AC_MSG_ERROR([awk not found])
    fi
    PHP_IMAGICK_FOUND_VERSION=`${PHP_CONFIG} --version`
    PHP_IMAGICK_FOUND_VERNUM=`echo "${PHP_IMAGICK_FOUND_VERSION}" | $AWK 'BEGIN { FS = "."; } { printf "%d", ([$]1 * 100 + [$]2) * 100 + [$]3;}'`
  else
    PHP_IMAGICK_FOUND_VERNUM="${PHP_VERSION_ID}"
    PHP_IMAGICK_FOUND_VERSION="${PHP_VERSION}"
  fi

  if test "$PHP_IMAGICK_FOUND_VERNUM" -ge "50103"; then
    AC_MSG_RESULT(yes. found $PHP_IMAGICK_FOUND_VERSION)
  else 
    AC_MSG_ERROR(no. found $PHP_IMAGICK_FOUND_VERSION)
  fi

  AC_DEFINE(HAVE_IMAGICK,1,[ ])

  IMAGICK_MAGICK_PREFIX=`$WAND_BINARY --prefix`
  
  export ORIG_PKG_CONFIG_PATH="$PKG_CONFIG_PATH"
  export PKG_CONFIG_PATH="$IMAGICK_MAGICK_PREFIX/lib/pkgconfig/"

  IMAGICK_LIBS=`$WAND_BINARY --libs`
  IMAGICK_LIBS="$IMAGICK_LIBS -L$WAND_DIR/lib"
  
  IMAGICK_CFLAGS=`$WAND_BINARY --cflags`
  IMAGICK_CFLAGS="$IMAGICK_CFLAGS -I$WAND_DIR/include"

  
  PHP_EVAL_LIBLINE($IMAGICK_LIBS, IMAGICK_SHARED_LIBADD)
  PHP_EVAL_INCLINE($IMAGICK_CFLAGS)

  export PKG_CONFIG_PATH="$ORIG_PKG_CONFIG_PATH"

  PHP_SUBST(IMAGICK_SHARED_LIBADD)
		
  PHP_NEW_EXTENSION(imagick, imagick_file.c imagick_class.c imagickdraw_class.c imagickpixel_class.c imagickpixeliterator_class.c imagick_helpers.c imagick.c, $ext_shared,, $IMAGICK_CFLAGS)
  PHP_INSTALL_HEADERS([ext/imagick], [php_imagick.h php_imagick_defs.h php_imagick_shared.h])
fi

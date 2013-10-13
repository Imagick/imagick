PHP_ARG_WITH(imagick, whether to enable the imagick extension,
[ --with-imagick[=DIR]	Enables the imagick extension. DIR is the prefix to Imagemagick installation directory.], no)

PHP_ARG_WITH(imagick-gm, whether to enable the imagick GraphicsMagick backend,
[ --with-imagick-gm[=DIR]	GraphicsMagick backend. NO LONGER SUPPORTED!], no, no)

PHP_ARG_ENABLE(imagick-zend-mm, whether to make Imagick respect PHP memory limits,
[ --enable-imagick-zend-mm	Make Imagick respect PHP memory limits], no, no)

if test $PHP_IMAGICK_GM != "no"; then
	AC_MSG_ERROR(GraphicsMagick backend is no longer supported)
fi

if test $PHP_IMAGICK != "no"; then

  AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
  if test "x$PKG_CONFIG" = "xno"; then
    AC_MSG_RESULT([pkg-config not found])
    AC_MSG_ERROR([Please reinstall the pkg-config distribution])
  fi

  if test -z "$AWK"; then
    AC_MSG_ERROR([awk not found])
  fi

  # ImageMagick has the config program:
  # bin/Wand-config
  # bin/MagickWand-config
  #
  # Sets IM_WAND_BINARY
  #
  AC_MSG_CHECKING(ImageMagick MagickWand API configuration program)

  if test "$PHP_IMAGICK" != "yes"; then
    export PKG_CONFIG_PATH="${PHP_IMAGICK}/lib/pkgconfig"

    for i in "${PHP_IMAGICK}" /usr/local /usr /opt /opt/local;
    do
      if test -r "${i}/bin/MagickWand-config"; then
        IM_WAND_BINARY="${i}/bin/MagickWand-config"
        break
      fi

      if test -r "${i}/bin/Wand-config"; then
        IM_WAND_BINARY="${i}/bin/Wand-config"
        break
      fi
    done
  else
    if test "x${PKG_CONFIG_PATH}" != "x"; then
      export PKG_CONFIG_PATH="${PKG_CONFIG_PATH}:/usr/local/lib/pkgconfig:/usr/lib/pkgconfig:/opt/lib/pkgconfig:/opt/local/lib/pkgconfig"
    else
      export PKG_CONFIG_PATH="/usr/local/lib/pkgconfig:/usr/lib/pkgconfig:/opt/lib/pkgconfig:/opt/local/lib/pkgconfig"
    fi

    for i in /usr/local /usr /opt /opt/local;
    do
      if test -r "${i}/bin/MagickWand-config"; then
        IM_WAND_BINARY="${i}/bin/MagickWand-config"
        break
      fi

      if test -r "${i}/bin/Wand-config"; then
        IM_WAND_BINARY="${i}/bin/Wand-config"
        break
      fi
    done
  fi

  if test "x" = "x$IM_WAND_BINARY"; then
    AC_MSG_ERROR(not found. Please provide a path to MagickWand-config or Wand-config program.)
  fi
  AC_MSG_RESULT([found in $IM_WAND_BINARY])

  # Check version
  # 
  IMAGEMAGICK_VERSION=`$IM_WAND_BINARY --version`
  IMAGEMAGICK_VERSION_MASK=`echo $IMAGEMAGICK_VERSION | $AWK 'BEGIN { FS = "."; } { printf "%d", ($1 * 1000 + $2) * 1000 + $3;}'`

  AC_MSG_CHECKING(if ImageMagick version is at least 6.2.4)
  if test "$IMAGEMAGICK_VERSION_MASK" -ge 6002004; then
    AC_MSG_RESULT(found version $IMAGEMAGICK_VERSION)
  else
    AC_MSG_ERROR(no. You need at least Imagemagick version 6.2.4 to use Imagick.)
  fi

  # Potential locations for the header
  # include/wand/magick-wand.h
  # include/wand/MagickWand.h
  # include/ImageMagick-6/wand/MagickWand.h
  # include/ImageMagick-7/MagickWand/MagickWand.h

  AC_MSG_CHECKING(for MagickWand.h or magick-wand.h header)

  IMAGEMAGICK_PREFIX=`$IM_WAND_BINARY --prefix`
  IMAGEMAGICK_MAJOR_VERSION=`echo $IMAGEMAGICK_VERSION | $AWK 'BEGIN { FS = "."; } {print $1}'`

  # Try the header formats from newest to oldest
  if test -r "${IMAGEMAGICK_PREFIX}/include/ImageMagick-${IMAGEMAGICK_MAJOR_VERSION}/MagickWand/MagickWand.h"; then

    AC_DEFINE([IMAGEMAGICK_HEADER_STYLE_SEVEN], [1], [ImageMagick 7 style header])
    AC_MSG_RESULT([${IMAGEMAGICK_PREFIX}/include/ImageMagick-${IMAGEMAGICK_MAJOR_VERSION}/MagickWand/MagickWand.h])

  elif test -r "${IMAGEMAGICK_PREFIX}/include/ImageMagick-${IMAGEMAGICK_MAJOR_VERSION}/wand/MagickWand.h"; then

    AC_DEFINE([IMAGEMAGICK_HEADER_STYLE_SIX], [1], [ImageMagick 6 style header])
    AC_MSG_RESULT([${IMAGEMAGICK_PREFIX}/include/ImageMagick-${IMAGEMAGICK_MAJOR_VERSION}/wand/MagickWand.h])

  elif test -r "${IMAGEMAGICK_PREFIX}/include/wand/MagickWand.h"; then

    AC_DEFINE([IMAGEMAGICK_HEADER_STYLE_SIX], [1], [ImageMagick 6 style header])
    AC_MSG_RESULT([${IMAGEMAGICK_PREFIX}/include/wand/MagickWand.h])

  elif test -r "${IMAGEMAGICK_PREFIX}/include/wand/magick-wand.h"; then

    AC_DEFINE([IMAGEMAGICK_HEADER_STYLE_OLD_OLD], [1], [Old old style header])
    AC_MSG_RESULT([${IMAGEMAGICK_PREFIX}/include/wand/magick-wand.h])

  else
    AC_MSG_ERROR([Unable to find MagickWand.h or magick-wand.h header])
  fi

#
# Zend MM
#
  AC_MSG_CHECKING([whether to use Zend MM])
  if test $PHP_IMAGICK_ZEND_MM != "no"; then
    AC_DEFINE(PHP_IMAGICK_ZEND_MM,1,[ ])
    AC_MSG_RESULT([yes])
  else
    AC_MSG_RESULT([no])
  fi

#
# PHP minimum version
#
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

#
# Set libs and CFLAGS for building
#
  IMAGICK_LIBS=`$IM_WAND_BINARY --libs`
  IMAGICK_LIBS="$IMAGICK_LIBS -L$WAND_DIR/lib"
  
  IMAGICK_CFLAGS=`$IM_WAND_BINARY --cflags`
  IMAGICK_CFLAGS="$IMAGICK_CFLAGS -I$WAND_DIR/include"

  PHP_EVAL_LIBLINE($IMAGICK_LIBS, IMAGICK_SHARED_LIBADD)
  PHP_EVAL_INCLINE($IMAGICK_CFLAGS)

  PHP_SUBST(IMAGICK_SHARED_LIBADD)
  AC_DEFINE(HAVE_IMAGICK,1,[ ])
  PHP_NEW_EXTENSION(imagick, imagick_file.c imagick_class.c imagickdraw_class.c imagickpixel_class.c imagickpixeliterator_class.c imagick_helpers.c imagick.c, $ext_shared,, $IMAGICK_CFLAGS)
  PHP_INSTALL_HEADERS([ext/imagick], [php_imagick.h php_imagick_defs.h php_imagick_shared.h])
fi

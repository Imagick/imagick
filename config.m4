
PHP_ARG_WITH(imagick, whether to enable the imagick extension,
[ --with-imagick[=DIR]	Enables the imagick extension. DIR is the prefix to Imagemagick installation directory.], no)

if test $PHP_IMAGICK != "no"; then

#########################################################
# IM_CHECK_PHP_VERSION()
#
# Check that the version of PHP meets the minimum requirements
#########################################################
AC_DEFUN([IM_CHECK_PHP_VERSION],[

  IM_MINIMUM_VERSION=$1
  IM_EXTRA_SEARCH_PREFIX=$2
  
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

]) #IM_CHECK_PHP_VERSION      


#########################################################
# IM_EVAL_LIBLINE_DEFER(libline, SHARED_LIBADD)
#
# Use this macro, if you need to add libraries and or library search
# paths to the PHP build system which are only given in compiler
# notation.
#########################################################
AC_DEFUN([IM_EVAL_LIBLINE_DEFER],[
  for ac_i in $1; do
    case $ac_i in
    -pthread[)]
      if test "$ext_shared" = "yes"; then
        $2="[$]$2 -pthread"
      else
        PHP_RUN_ONCE(EXTRA_LDFLAGS, [$ac_i], [EXTRA_LDFLAGS="$EXTRA_LDFLAGS $ac_i"])
      fi
    ;;
    -l*[)]
      ac_ii=`echo $ac_i|cut -c 3-`
      PHP_ADD_LIBRARY_DEFER($ac_ii,1,$2)
    ;;
    -L*[)]
      ac_ii=`echo $ac_i|cut -c 3-`
      AC_MSG_NOTICE(cut var is $ac_ii)
      PHP_ADD_LIBPATH($ac_ii,$2)
    ;;
    esac
  done
]) #IM_EVAL_LIBLINE_DEFER      


#########################################################
# IM_CHECK_IMAGEMAGICK_VERSION(IM_MINIMUM_VERSION, IM_FOUND_VERSION)
#
# Use this macro, if you need to add libraries and or library search
# paths to the PHP build system which are only given in compiler
# notation.
#########################################################
AC_DEFUN([IM_CHECK_IMAGEMAGICK_VERSION],[

  IM_MINIMUM_VERSION=$1
  IM_FOUND_VERSION=$2
  
    IM_IMAGEMAGICK_VERSION_MASK=`echo $IM_IMAGEMAGICK_VERSION | $AWK 'BEGIN { FS = "."; } { printf "%d", ($[1] * 1000 + $[2]) * 1000 + $[3];}'`

  IM_MIMIMUM_VERSION_MASK=`echo $IM_MINIMUM_VERSION | $AWK 'BEGIN { FS = "."; } { printf "%d", ($[1] * 1000 + $[2]) * 1000 + $[3];}'`

  AC_MSG_CHECKING(if ImageMagick version is at least $IM_MINIMUM_VERSION)
  if test "$IM_IMAGEMAGICK_VERSION_MASK" -ge $IM_MIMIMUM_VERSION_MASK; then
    AC_MSG_RESULT(found version $IM_IMAGEMAGICK_VERSION)
  else
    AC_MSG_ERROR(no. You need at least Imagemagick version $IM_MINIMUM_VERSION to use this extension.)
  fi

]) #IM_CHECK_IMAGEMAGICK_VERSION      

#########################################################
# IM_LOCATE_HEADER_AND_STYLE()
#
# Figure out where the header file is and determine what 'style'
# it should be included as
#########################################################
AC_DEFUN([IM_LOCATE_HEADER_AND_STYLE],[

  # Potential locations for the header
  # include/wand/magick-wand.h
  # include/ImageMagick/wand/MagickWand.h
  # include/ImageMagick-6/wand/MagickWand.h
  # include/ImageMagick-7/MagickWand/MagickWand.h
  AC_MSG_CHECKING(for MagickWand.h or magick-wand.h header)

  # Try the header formats from newest to oldest
  if test -r "${IM_IMAGEMAGICK_PREFIX}/include/ImageMagick-${IM_MAJOR_VERSION}/wand/MagickWand.h"; then
  
    IM_INCLUDE_FORMAT="wand/MagickWand.h"
    IM_HEADER_STYLE="SIX"
    AC_DEFINE([IM_MAGICKWAND_HEADER_STYLE_SIX], [1], [ImageMagick 6.x style header])
  
    AC_MSG_RESULT([user location ${IM_IMAGEMAGICK_PREFIX}/include/ImageMagick-${IM_MAJOR_VERSION}/wand/MagickWand.h])

  elif test -r "${IM_PREFIX}/include/ImageMagick-${IM_MAJOR_VERSION}/MagickWand/MagickWand.h"; then

    IM_INCLUDE_FORMAT="MagickWand/MagickWand.h"
    IM_HEADER_STYLE="SEVEN"
    AC_DEFINE([IM_MAGICKWAND_HEADER_STYLE_SEVEN], [1], [ImageMagick 7.x style header])

    AC_MSG_RESULT([${IM_PREFIX}/include/ImageMagick-${IM_MAJOR_VERSION}/MagickWand/MagickWand.h])

  elif test -r "${IM_PREFIX}/include/ImageMagick-${IM_MAJOR_VERSION}/wand/MagickWand.h"; then

    IM_INCLUDE_FORMAT="wand/MagickWand.h"
    IM_HEADER_STYLE="SIX"
    AC_DEFINE([IM_MAGICKWAND_HEADER_STYLE_SIX], [1], [ImageMagick 6.x style header])

    AC_MSG_RESULT([${IM_PREFIX}/include/ImageMagick-${IM_MAJOR_VERSION}/wand/MagickWand.h])

  elif test -r "${IM_PREFIX}/include/ImageMagick/wand/MagickWand.h"; then

    IM_INCLUDE_FORMAT="wand/MagickWand.h"
    IM_HEADER_STYLE="SIX"
    AC_DEFINE([IM_MAGICKWAND_HEADER_STYLE_SIX], [1], [ImageMagick 6.x style header])

    AC_MSG_RESULT([${IM_PREFIX}/include/ImageMagick/wand/MagickWand.h])

  elif test -r "${IM_PREFIX}/include/ImageMagick/wand/magick-wand.h"; then

    IM_INCLUDE_FORMAT="wand/magick-wand.h"
    IM_HEADER_STYLE="OLD"
    AC_DEFINE([IM_MAGICKWAND_HEADER_STYLE_OLD], [1], [ImageMagick old style header])

    AC_MSG_RESULT([${IM_PREFIX}/include/wand/magick-wand.h])

  else
    AC_MSG_ERROR([Unable to find MagickWand.h or magick-wand.h header])
  fi

]) #IM_LOCATE_HEADER_AND_STYLE


#########################################################
# Locate ImageMagick configuration program
# ImageMagick has the config program:
# bin/Wand-config
# bin/MagickWand-config
#
# Sets
#  IM_WAND_BINARY
#  IM_IMAGEMAGICK_PREFIX
#  IM_IMAGEMAGICK_VERSION
#  IM_IMAGEMAGICK_VERSION_MASK
#  IM_USE_PKG_CONFIG
#
# Usage:
#   IM_FIND_IMAGEMAGICK (MINIMUM_VERSION, EXTRA_SEARCH_PREFIX)
#
# Note:
# IM_PREFIX - the directory reported by `MagickWand-config --prefix`
# IM_IMAGEMAGICK_PREFIX - the user specified location to use
#
#########################################################
AC_DEFUN([IM_FIND_IMAGEMAGICK],[

  # Read variables passed in
  IM_MINIMUM_VERSION=$1
  IM_EXTRA_SEARCH_PREFIX=$2

  AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
  if test "x$PKG_CONFIG" = "xno"; then
    AC_MSG_RESULT([pkg-config not found])
    AC_MSG_ERROR([Please reinstall the pkg-config distribution])
  fi

  # awk is required below. Error if it is not available.
  if test -z "$AWK"; then
    AC_MSG_ERROR([awk not found])
  fi

  AC_MSG_NOTICE(Trying to locate ImageMagick MagickWand API configuration program)

  #IM_LOCATIONS=("/usr/local" "/usr" "/usr/sbin" "/opt" "/opt/local")
  #if test "$IM_EXTRA_SEARCH_PREFIX" != "yes"; then
  #  IM_LOCATIONS=("$IM_EXTRA_SEARCH_PREFIX" "/usr/local" "/usr" "/usr/sbin" "/opt" "/opt/local")
  #fi

  for i in /usr/local /usr /usr/sbin /opt /opt/local;
  do  
    AC_MSG_CHECKING([Testing ${i}/bin/MagickWand-config])
    if test -r "${i}/bin/MagickWand-config"; then
      IM_WAND_BINARY="${i}/bin/MagickWand-config"
      IM_IMAGEMAGICK_PREFIX=$i
      AC_MSG_RESULT([It exists])
      break
    fi

    if test -r "${i}/bin/Wand-config"; then
      IM_WAND_BINARY="${i}/bin/Wand-config"
      IM_IMAGEMAGICK_PREFIX=$i
      AC_MSG_RESULT([It exists])
      break
    fi
    AC_MSG_RESULT([Doesn't exist])
  done

  # If we have failed to locate ImageMagick automatically, lets try to locate it
  # via 
  IM_USE_PKG_CONFIG=0
  if test "x" = "x$IM_WAND_BINARY"; then
    if eval '$PKG_CONFIG --exists MagickWand'; then
      IM_USE_PKG_CONFIG=1
      IM_WAND_BINARY=$PKG_CONFIG
    else
      AC_MSG_ERROR(not found. Please provide a path to MagickWand-config or Wand-config program.)
    fi
  fi
  AC_MSG_RESULT([found in $IM_WAND_BINARY])

  # This is used later for cflags and libs
  export PKG_CONFIG_PATH="${IM_IMAGEMAGICK_PREFIX}/${PHP_LIBDIR}/pkgconfig"
  
  # Check version
  if test "$IM_USE_PKG_CONFIG" = "1"; then
    IM_IMAGEMAGICK_VERSION=`$IM_WAND_BINARY --modversion MagickWand`
  else
    IM_IMAGEMAGICK_VERSION=`$IM_WAND_BINARY --version`
  fi

  IM_MAJOR_VERSION=`echo $IM_IMAGEMAGICK_VERSION | $AWK 'BEGIN { FS = "."; } {print $[1]}'`  
  IM_CHECK_IMAGEMAGICK_VERSION($IM_IMAGEMAGICK_VERSION, $IM_MINIMUM_VERSION)

  if test "$IM_USE_PKG_CONFIG" = "1"; then
    IM_PREFIX=`$IM_WAND_BINARY --variable prefix MagickWand`
  else
    IM_PREFIX=`$IM_WAND_BINARY --prefix`
  fi

  IM_LOCATE_HEADER_AND_STYLE()

  # The cflags and libs
  if test "$IM_USE_PKG_CONFIG" = "1"; then
    IM_IMAGEMAGICK_LIBS=`$IM_WAND_BINARY --libs MagickWand`
    IM_IMAGEMAGICK_CFLAGS=`$IM_WAND_BINARY --cflags MagickWand`
  else
    IM_IMAGEMAGICK_LIBS=`$IM_WAND_BINARY --libs`
    IM_IMAGEMAGICK_CFLAGS=`$IM_WAND_BINARY --cflags`
  fi

  AC_MSG_RESULT([Imagick m4 exports are:])
  AC_MSG_RESULT([IM_IMAGEMAGICK_PREFIX: $IM_IMAGEMAGICK_PREFIX])
  AC_MSG_RESULT([IM_WAND_BINARY: $IM_WAND_BINARY])
  AC_MSG_RESULT([IM_IMAGEMAGICK_VERSION: $IM_IMAGEMAGICK_VERSION])
  AC_MSG_RESULT([IM_IMAGEMAGICK_VERSION_MASK: $IM_IMAGEMAGICK_VERSION_MASK])
  AC_MSG_RESULT([IM_INCLUDE_FORMAT: $IM_INCLUDE_FORMAT])
  AC_MSG_RESULT([IM_HEADER_STYLE: $IM_HEADER_STYLE])
  AC_MSG_RESULT([IM_USE_PKG_CONFIG: $IM_USE_PKG_CONFIG])
  AC_MSG_RESULT([IM_IMAGEMAGICK_LIBS: $IM_IMAGEMAGICK_LIBS])
  AC_MSG_RESULT([IM_IMAGEMAGICK_CFLAGS: $IM_IMAGEMAGICK_CFLAGS])
  
  export IM_IMAGEMAGICK_PREFIX
  export IM_WAND_BINARY
  export IM_IMAGEMAGICK_VERSION
  export IM_IMAGEMAGICK_VERSION_MASK
  export IM_INCLUDE_FORMAT
  export IM_HEADER_STYLE
  export IM_USE_PKG_CONFIG

  export IM_IMAGEMAGICK_LIBS
  export IM_IMAGEMAGICK_CFLAGS

]) #IM_FIND_IMAGEMAGICK






#########################################################
# Beginning of actual execution.
#########################################################
  if test "$ext_shared" != "yes" && test "$ext_shared" != "shared"; then
    define('PHP_IMAGICK_STATIC', 1)
    PHP_IMAGICK_STATIC=yes
  fi
 
  #m4_include(ifdef('PHP_IMAGICK_STATIC',PHP_EXT_BUILDDIR(imagick)[/],)[imagemagick.m4])

  # Find where ImageMagick is located.
  IM_FIND_IMAGEMAGICK([6.2.4], [$PHP_IMAGICK])

  # PHP minimum version
  IM_CHECK_PHP_VERSION()

  # Set libs and CFLAGS for building
  AC_MSG_RESULT(IM_IMAGEMAGICK_LIBS found $IM_IMAGEMAGICK_LIBS)
  AC_MSG_RESULT(IMAGICK_SHARED_LIBADD found $IMAGICK_SHARED_LIBADD)
  
  # Pass the libs and include strings to PHP
  # PHP_EVAL_LIBLINE($IM_IMAGEMAGICK_LIBS, IMAGICK_SHARED_LIBADD)
  
  if test $PHP_IMAGICK_STATIC = "yes"; then
    # We don't want the imagick libs added to the probe programs compiled
    # by configure
    AC_MSG_NOTICE([Imagick compiling statically, adding libs to link path])
    IM_EVAL_LIBLINE_DEFER($IM_IMAGEMAGICK_LIBS, IMAGICK_SHARED_LIBADD)
  else 
    AC_MSG_NOTICE([Imagick compiling as extension - add libs everything])
    PHP_EVAL_LIBLINE($IM_IMAGEMAGICK_LIBS, IMAGICK_SHARED_LIBADD)
  fi
  
  PHP_EVAL_INCLINE($IM_IMAGEMAGICK_CFLAGS)

  # Adds variable with it's value into Makefile, e.g.:
  PHP_SUBST(IMAGICK_SHARED_LIBADD)
  AC_DEFINE(HAVE_IMAGICK,1,[ ])
  
  # Adds the files to be compiled
  PHP_NEW_EXTENSION(imagick, imagick_file.c imagick_class.c imagickdraw_class.c imagickpixel_class.c imagickpixeliterator_class.c imagick_helpers.c imagick.c imagickkernel_class.c shim_im6_to_im7.c, $ext_shared,, $IM_IMAGEMAGICK_CFLAGS)

  # Adds the heades to be available
  PHP_INSTALL_HEADERS([ext/imagick], [php_imagick_shared.h])
fi

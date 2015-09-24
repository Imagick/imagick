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

dnl
dnl PHP_EVAL_LIBLINE(libline, SHARED-LIBADD)
dnl
dnl Use this macro, if you need to add libraries and or library search
dnl paths to the PHP build system which are only given in compiler
dnl notation.
dnl
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
      PHP_ADD_LIBPATH_DEFER($ac_ii,$2)
    ;;
    esac
  done
])

AC_DEFUN([IM_FIND_IMAGEMAGICK],[

#
# Variables passed in
#
  IM_MINIMUM_VERSION=$1
  IM_EXTRA_SEARCH_PREFIX=$2

  AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
  if test "x$PKG_CONFIG" = "xno"; then
    AC_MSG_RESULT([pkg-config not found])
    AC_MSG_ERROR([Please reinstall the pkg-config distribution])
  fi

  if test -z "$AWK"; then
    AC_MSG_ERROR([awk not found])
  fi

  AC_MSG_CHECKING(ImageMagick MagickWand API configuration program)

  if test "$IM_EXTRA_SEARCH_PREFIX" != "yes"; then
    for i in "$IM_EXTRA_SEARCH_PREFIX" /usr/local /usr /usr/sbin /opt /opt/local;
    do
      AC_MSG_CHECKING([Testing ${i}/bin/MagickWand-config])
      if test -r "${i}/bin/MagickWand-config"; then
        IM_WAND_BINARY="${i}/bin/MagickWand-config"
        IM_IMAGEMAGICK_PREFIX=$i
        AC_MSG_RESULT([It exists])
        break
      fi
      AC_MSG_RESULT([Doesn't exist])

      AC_MSG_CHECKING([Testing ${i}/bin/Wand-config])
      if test -r "${i}/bin/Wand-config"; then
        IM_WAND_BINARY="${i}/bin/Wand-config"
        IM_IMAGEMAGICK_PREFIX=$i
        AC_MSG_RESULT([It exists])
        break
      fi
      AC_MSG_RESULT([Doesn't exist])
    done
  else
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
  fi

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
#
  if test "$IM_USE_PKG_CONFIG" = "1"; then
    IM_IMAGEMAGICK_VERSION=`$IM_WAND_BINARY --modversion MagickWand`
  else
    IM_IMAGEMAGICK_VERSION=`$IM_WAND_BINARY --version`
  fi
  IM_IMAGEMAGICK_VERSION_MASK=`echo $IM_IMAGEMAGICK_VERSION | $AWK 'BEGIN { FS = "."; } { printf "%d", ($[1] * 1000 + $[2]) * 1000 + $[3];}'`

  IM_MIMIMUM_VERSION_MASK=`echo $IM_MINIMUM_VERSION | $AWK 'BEGIN { FS = "."; } { printf "%d", ($[1] * 1000 + $[2]) * 1000 + $[3];}'`

  AC_MSG_CHECKING(if ImageMagick version is at least $IM_MINIMUM_VERSION)
  if test "$IM_IMAGEMAGICK_VERSION_MASK" -ge $IM_MIMIMUM_VERSION_MASK; then
    AC_MSG_RESULT(found version $IM_IMAGEMAGICK_VERSION)
  else
    AC_MSG_ERROR(no. You need at least Imagemagick version $IM_MINIMUM_VERSION to use this extension.)
  fi

# Potential locations for the header
# include/wand/magick-wand.h
# include/ImageMagick/wand/MagickWand.h
# include/ImageMagick-6/wand/MagickWand.h
# include/ImageMagick-7/MagickWand/MagickWand.h

  AC_MSG_CHECKING(for MagickWand.h or magick-wand.h header)

  if test "$IM_USE_PKG_CONFIG" = "1"; then
    IM_PREFIX=`$IM_WAND_BINARY --variable prefix MagickWand`
  else
    IM_PREFIX=`$IM_WAND_BINARY --prefix`
  fi
  IM_MAJOR_VERSION=`echo $IM_IMAGEMAGICK_VERSION | $AWK 'BEGIN { FS = "."; } {print $[1]}'`

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
  
#
# The cflags and libs
#
  if test "$IM_USE_PKG_CONFIG" = "1"; then
    IM_IMAGEMAGICK_LIBS=`$IM_WAND_BINARY --libs MagickWand`
    IM_IMAGEMAGICK_CFLAGS=`$IM_WAND_BINARY --cflags MagickWand`
  else
    IM_IMAGEMAGICK_LIBS=`$IM_WAND_BINARY --libs`
    IM_IMAGEMAGICK_CFLAGS=`$IM_WAND_BINARY --cflags`
  fi
  export IM_IMAGEMAGICK_PREFIX
  export IM_WAND_BINARY
  export IM_IMAGEMAGICK_VERSION
  export IM_IMAGEMAGICK_VERSION_MASK
  export IM_INCLUDE_FORMAT
  export IM_HEADER_STYLE
  export IM_USE_PKG_CONFIG

  export IM_IMAGEMAGICK_LIBS
  export IM_IMAGEMAGICK_CFLAGS
])


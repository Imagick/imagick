#########################################################
# Locate ImageMagick
#
# Sets
#  IM_IMAGEMAGICK_PREFIX
#  IM_IMAGEMAGICK_VERSION
#  IM_IMAGEMAGICK_VERSION_MASK
#
# Usage:
#   IM_FIND_IMAGEMAGICK (MINIMUM_VERSION, EXTRA_SEARCH_PREFIX)
#
#########################################################

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

# This is used later for cflags and libs
  export PKG_CONFIG_PATH="${IM_IMAGEMAGICK_PREFIX}/${PHP_LIBDIR}/pkgconfig"
  
# Check version
#
  IM_IMAGEMAGICK_VERSION=`$PKG_CONFIG --modversion MagickWand`
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

  IM_PREFIX=`$PKG_CONFIG --variable=prefix MagickWand`
  IM_MAJOR_VERSION=`echo $IM_IMAGEMAGICK_VERSION | $AWK 'BEGIN { FS = "."; } {print $[1]}'`

  # Try the header formats from newest to oldest
  if test -r "${IM_PREFIX}/include/ImageMagick-${IM_MAJOR_VERSION}/MagickWand/MagickWand.h"; then

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
  IM_IMAGEMAGICK_LIBS=`$PKG_CONFIG --libs MagickWand`
  IM_IMAGEMAGICK_CFLAGS=`$PKG_CONFIG --cflags MagickWand`

  export IM_IMAGEMAGICK_PREFIX
  export IM_IMAGEMAGICK_VERSION
  export IM_IMAGEMAGICK_VERSION_MASK
  export IM_INCLUDE_FORMAT
  export IM_HEADER_STYLE

  export IM_IMAGEMAGICK_LIBS
  export IM_IMAGEMAGICK_CFLAGS
])


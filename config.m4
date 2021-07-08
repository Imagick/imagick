PHP_ARG_WITH(imagick, whether to enable the imagick extension,
[ --with-imagick[=DIR]	Enables the imagick extension. DIR is the prefix to ImageMagick installation directory.], no)


if test $PHP_IMAGICK != "no"; then

#
# Find ImageMagick
#
if test "$ext_shared" != "yes" && test "$ext_shared" != "shared"; then
  define('PHP_IMAGICK_STATIC', 1)
  PHP_IMAGICK_STATIC=yes
fi
m4_include(ifdef('PHP_IMAGICK_STATIC',PHP_EXT_BUILDDIR(imagick)[/],)[imagemagick.m4])

# This line checks that ImageMagick is available, and
# meets our minimum supported version. TODO change to 6.7.0
IM_FIND_IMAGEMAGICK([6.2.4], [$PHP_IMAGICK])

#
# PHP minimum version
#
  AC_MSG_CHECKING([PHP version is at least 5.3.2])

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

  if test "$PHP_IMAGICK_FOUND_VERNUM" -ge "50302"; then
    AC_MSG_RESULT(yes. found $PHP_IMAGICK_FOUND_VERSION)
  else
    AC_MSG_ERROR(no. found $PHP_IMAGICK_FOUND_VERSION)
  fi

#
# Set libs and CFLAGS for building
#
	echo "libs"
	echo $IM_IMAGEMAGICK_LIBS
	echo ""
	echo $IMAGICK_SHARED_LIBADD

  PHP_EVAL_LIBLINE($IM_IMAGEMAGICK_LIBS, IMAGICK_SHARED_LIBADD)
  PHP_EVAL_INCLINE($IM_IMAGEMAGICK_CFLAGS)

  old_LIBS=$LIBS
  LIBS="$LIBS $IM_IMAGEMAGICK_LIBS"

  PHP_CHECK_FUNC(MagickGetVersion)

  LIBS=$old_LIBS

  AC_MSG_CHECKING(omp_pause_resource_all usability)
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
    #include <omp.h>
  ]],[[
    omp_pause_resource_all(omp_pause_hard);
  ]])],[
    AC_MSG_RESULT(yes)
    AC_MSG_CHECKING([Which flavour of OpenMP to link])

    if test "${GCC}" = "yes"; then
      # Open64 (passes for GCC but uses different OpenMP implementation)
      if test "x$GOMP_LIBS" = x ; then
        if $CC --version 2>&1 | grep Open64 > /dev/null ; then
          PHP_CHECK_FUNC(omp_pause_resource_all, openmp)
          if test "x$ac_cv_func_omp_pause_resource_all" = "xyes"; then
            PHP_ADD_LIBRARY(openmp,, IMAGICK_SHARED_LIBADD)
            AC_MSG_RESULT([Open64 flavoured OpenMP])
          fi
        fi
      fi
      # Clang (passes for GCC but uses different OpenMP implementation)
      if test "x$LIB_OMP" = x ; then
        if $CC --version 2>&1 | grep clang > /dev/null ; then
          PHP_CHECK_FUNC(omp_pause_resource_all, omp)
          if test "x$ac_cv_func_omp_pause_resource_all" = "xyes"; then
            PHP_ADD_LIBRARY(omp,, IMAGICK_SHARED_LIBADD)
            AC_MSG_RESULT([Clang flavoured OpenMP])
          fi
        fi
      fi
      # GCC
      if test "x$ac_cv_func_omp_pause_resource_all" = x ; then
        PHP_CHECK_FUNC(omp_pause_resource_all, gomp)
        if test "x$ac_cv_func_omp_pause_resource_all" = "xyes"; then
          PHP_ADD_LIBRARY(gomp,, IMAGICK_SHARED_LIBADD)
          AC_MSG_RESULT([GCC flavoured OpenMP])
        fi
      fi
    fi

  ],[
    AC_MSG_RESULT(no)
  ])

  PHP_SUBST(IMAGICK_SHARED_LIBADD)
  AC_DEFINE(HAVE_IMAGICK,1,[ ])
  PHP_NEW_EXTENSION(imagick, imagick_file.c imagick_class.c imagickdraw_class.c imagickpixel_class.c imagickpixeliterator_class.c imagick_helpers.c imagick.c imagickkernel_class.c shim_im6_to_im7.c, $ext_shared,, $IM_IMAGEMAGICK_CFLAGS)
  PHP_INSTALL_HEADERS([ext/imagick], [php_imagick_shared.h])
fi

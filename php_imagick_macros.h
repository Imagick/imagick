/*
   +----------------------------------------------------------------------+
   | PHP Version 5 / Imagick	                                          |
   +----------------------------------------------------------------------+
   | Copyright (c) 2006-2013 Mikko Koppanen, Scott MacVicar               |
   | Imagemagick (c) ImageMagick Studio LLC                               |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Mikko Kopppanen <mkoppanen@php.net>                          |
   |         Scott MacVicar <scottmac@php.net>                            |
   +----------------------------------------------------------------------+
*/

#ifndef PHP_IMAGICK_MACROS_H
# define PHP_IMAGICK_MACROS_H

#define IMAGICK_FREE_MAGICK_MEMORY(value) \
	do { \
		if (value) { \
			MagickRelinquishMemory(value); \
			value = NULL; \
		} \
	} while (0)

#if !defined(E_DEPRECATED)
#  define E_DEPRECATED E_STRICT
#endif

#define IMAGICK_METHOD_DEPRECATED(class_name, method_name) \
	php_error(E_DEPRECATED, "%s::%s method is deprecated and it's use should be avoided", class_name, method_name);

#define IMAGICK_METHOD_DEPRECATED_USE_INSTEAD(class_name, method_name, new_class, new_method) \
	php_error(E_DEPRECATED, "%s::%s is deprecated. %s::%s should be used instead", class_name, method_name, new_class, new_method);


#define IMAGICK_KERNEL_NOT_NULL_EMPTY(kernel) \
	if (kernel->kernel_info == NULL) { \
		zend_throw_exception(php_imagickkernel_exception_class_entry, "ImagickKernel is empty, cannot be used", (long)0 TSRMLS_CC); \
		RETURN_NULL(); \
	}

#endif /* PHP_IMAGICK_MACROS_H */

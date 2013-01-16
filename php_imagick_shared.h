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

#ifndef _PHP_IMAGICK_SHARED_H_
# define _PHP_IMAGICK_SHARED_H_

/* Only works with newer Imagick */
#ifndef IMAGICK_USE_NEW_HEADER
# define IMAGICK_USE_NEW_HEADER 1
#endif

#include "php_imagick.h"
#include "php_imagick_defs.h"

/*
	Allow extensions to use the class entries
*/
MY_IMAGICK_EXPORTS zend_class_entry *php_imagick_get_class_entry();
MY_IMAGICK_EXPORTS zend_class_entry *php_imagickdraw_get_class_entry();
MY_IMAGICK_EXPORTS zend_class_entry *php_imagickpixel_get_class_entry();

#endif

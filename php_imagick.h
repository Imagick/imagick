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
#ifndef PHP_IMAGICK_H
/* Prevent double inclusion */
#define PHP_IMAGICK_H

/* Define Extension Properties */
#define PHP_IMAGICK_EXTNAME    "imagick"
// The version is deliberately left as 'PACKAGE_VERSION' in source code.
// It is only replaced with the actual version number that packaged through pecl.php.net
#define PHP_IMAGICK_VERSION    "@PACKAGE_VERSION@"
#define PHP_IMAGICK_EXTNUM     30403

/* Import configure options when building 
   outside of the PHP source tree */
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef ZTS
# include "TSRM.h"
#endif

/* Include PHP Standard Headers */
#include "php.h"

/* Define the entry point symbol
 * ZE will use when loading this module
 */
extern zend_module_entry imagick_module_entry;
#define phpext_imagick_ptr &imagick_module_entry

#endif /* PHP_IMAGICK_H */


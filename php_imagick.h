/*
   +----------------------------------------------------------------------+
   | PHP version 4.0                                                      |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997, 1998, 1999, 2000, 2001 The PHP Group             |
   +----------------------------------------------------------------------+
   | This source file is subject to version 2.02 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available at through the world-wide-web at                           |
   | http://www.php.net/license/2_02.txt.                                 |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Authors:                                                             |
   |                                                                      |
   +----------------------------------------------------------------------+
 */


#ifndef PHP_IMAGICK_H
#define PHP_IMAGICK_H

extern zend_module_entry imagick_module_entry;
#define phpext_imagick_ptr &imagick_module_entry

#ifdef PHP_WIN32
#define PHP_IMAGICK_API __declspec(dllexport)
#else
#define PHP_IMAGICK_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#include <magick/api.h>


PHP_MINIT_FUNCTION(imagick);
PHP_MSHUTDOWN_FUNCTION(imagick);
PHP_RINIT_FUNCTION(imagick);
PHP_RSHUTDOWN_FUNCTION(imagick);
PHP_MINFO_FUNCTION(imagick);

PHP_FUNCTION(confirm_imagick_compiled);	/* For testing, remove later. */

PHP_FUNCTION(imagick_read);
PHP_FUNCTION(imagick_sample);
PHP_FUNCTION(imagick_resize);
PHP_FUNCTION(imagick_crop);
PHP_FUNCTION(imagick_rotate);
PHP_FUNCTION(imagick_shear);
PHP_FUNCTION(imagick_oilpaint);
PHP_FUNCTION(imagick_annotate);
PHP_FUNCTION(imagick_morph);
PHP_FUNCTION(imagick_write);
PHP_FUNCTION(imagick_list_magickinfo);
PHP_FUNCTION(imagick_dump);
PHP_FUNCTION(imagick_set_attribute);
PHP_FUNCTION(imagick_get_attribute);
PHP_FUNCTION(imagick_new);
PHP_FUNCTION(imagick_convert);
PHP_FUNCTION(imagick_free);
PHP_FUNCTION(imagick_init);



typedef struct {

  ImageInfo *info;
  Image *image;
  int count;
} php_imagick;

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     
*/

ZEND_BEGIN_MODULE_GLOBALS(imagick)
	int   global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(imagick)


/* In every utility function you add that needs to use variables 
   in php_imagick_globals, call TSRM_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMG_CC
   after the last function argument and declare your utility function
   with TSRMG_DC after the last declared argument.  Always refer to
   the globals in your function as IMAGICK_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define IMAGICK_G(v) TSRMG(imagick_globals_id, zend_imagick_globals *, v)
#else
#define IMAGICK_G(v) (imagick_globals.v)
#endif

#endif	/* PHP_IMAGICK_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * indent-tabs-mode: t
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */


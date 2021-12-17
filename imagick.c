/*
   +----------------------------------------------------------------------+
   | PHP Version 5 / Imagick	                                          |
   +----------------------------------------------------------------------+
   | Copyright (c) 2006-2013 Mikko Koppanen, Scott MacVicar               |
   | ImageMagick (c) ImageMagick Studio LLC                               |
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

#include "php_imagick.h"
#include "php_imagick_defs.h"
#include "php_imagick_macros.h"
#include "php_imagick_helpers.h"
#include "php_imagick_shared.h"

#if PHP_VERSION_ID >= 70000
#include "ext/standard/php_smart_string.h"
#else
#include "ext/standard/php_smart_str.h"
#endif

#if PHP_VERSION_ID < 70200
///* For the countable interface */
#include "ext/spl/spl_iterators.h"
#endif

#include "shim_php7_to_php8.h"

#if HAVE_OMP_PAUSE_RESOURCE_ALL
#include <omp.h>
#endif

ZEND_DECLARE_MODULE_GLOBALS(imagick)

#ifdef IMAGICK_WITH_KERNEL
	#if PHP_VERSION_ID >= 80000
	HashTable* php_imagickkernel_get_debug_info(zend_object *obj, int *is_temp TSRMLS_DC); /* {{{ */
	#else
	HashTable* php_imagickkernel_get_debug_info(zval *obj, int *is_temp TSRMLS_DC); /* {{{ */
	#endif
#endif

#if MAGICKCORE_HDRI_ENABLE
	#define IMAGICK_QUANTUM_TYPE IS_DOUBLE
#else
	#define IMAGICK_QUANTUM_TYPE IS_LONG
#endif

zend_class_entry *php_imagick_sc_entry;
zend_class_entry *php_imagick_exception_class_entry;
zend_class_entry *php_imagickdraw_sc_entry;
zend_class_entry *php_imagickdraw_exception_class_entry;
zend_class_entry *php_imagickpixel_sc_entry;
zend_class_entry *php_imagickpixel_exception_class_entry;
zend_class_entry *php_imagickpixeliterator_sc_entry;
zend_class_entry *php_imagickpixeliterator_exception_class_entry;
#ifdef IMAGICK_WITH_KERNEL
zend_class_entry *php_imagickkernel_sc_entry;
zend_class_entry *php_imagickkernel_exception_class_entry;
#endif

#if defined(ZTS) && defined(PHP_WIN32)
static MUTEX_T imagick_mutex;
static THREAD_T imagick_thread_id;
#endif

/* Handlers */
static zend_object_handlers imagick_object_handlers;
static zend_object_handlers imagickdraw_object_handlers;
static zend_object_handlers imagickpixel_object_handlers;
static zend_object_handlers imagickpixeliterator_object_handlers;
#ifdef IMAGICK_WITH_KERNEL
static zend_object_handlers imagickkernel_object_handlers;
#endif

/* External API */
PHP_IMAGICK_API zend_class_entry *php_imagick_get_class_entry()
{
	return php_imagick_sc_entry;
}

PHP_IMAGICK_API zend_class_entry *php_imagickdraw_get_class_entry()
{
	return php_imagickdraw_sc_entry;
}

PHP_IMAGICK_API zend_class_entry *php_imagickpixel_get_class_entry()
{
	return php_imagickpixel_sc_entry;
}

static zend_function_entry php_imagick_functions[] =
{
	{ NULL, NULL, NULL, 0, 0 }
};


#include "ImagickDraw_arginfo.h"
const zend_function_entry *php_imagickdraw_class_methods = class_ImagickDraw_methods;

#include "ImagickPixelIterator_arginfo.h"
const zend_function_entry *php_imagickpixeliterator_class_methods = class_ImagickPixelIterator_methods;

#include "ImagickPixel_arginfo.h"
const zend_function_entry *php_imagickpixel_class_methods = class_ImagickPixel_methods;

#include "Imagick_arginfo.h"
const zend_function_entry *php_imagick_class_methods = class_Imagick_methods;

#ifdef IMAGICK_WITH_KERNEL
#include "ImagickKernel_arginfo.h"
const zend_function_entry *php_imagickkernel_class_methods = class_ImagickKernel_methods;
#endif

static void php_imagick_object_free_storage(IM_ZEND_OBJECT *object TSRMLS_DC)
{
	php_imagick_object *intern = php_imagick_fetch_object(object);

	if (!intern) {
		return;
	}

	if (intern->progress_monitor_name) {
		//TODO - is this meant to be free by us? 
		efree(intern->progress_monitor_name);
	}

	if (intern->magick_wand != NULL) {
		intern->magick_wand = DestroyMagickWand(intern->magick_wand);
	}

	zend_object_std_dtor(&intern->zo TSRMLS_CC);
#if PHP_VERSION_ID < 70000
	efree(intern);
#endif
}


static void php_imagickdraw_object_free_storage(IM_ZEND_OBJECT *object TSRMLS_DC)
{
	php_imagickdraw_object *intern = php_imagickdraw_fetch_object(object);

	if (!intern) {
		return;
	}

	if (intern->drawing_wand != NULL) {
		intern->drawing_wand = DestroyDrawingWand(intern->drawing_wand);
	}

	zend_object_std_dtor(&intern->zo TSRMLS_CC);

	#if PHP_VERSION_ID < 70000
		efree(intern);
	#endif
}

static void php_imagickpixeliterator_object_free_storage(IM_ZEND_OBJECT *object TSRMLS_DC)
{
	php_imagickpixeliterator_object *intern = php_imagickpixeliterator_fetch_object(object);

	if (!intern) {
		return;
	}

	if (intern->pixel_iterator != NULL) {
		intern->pixel_iterator = DestroyPixelIterator(intern->pixel_iterator);
	}

	zend_object_std_dtor(&intern->zo TSRMLS_CC);
	#if PHP_VERSION_ID < 70000
		efree(intern);
	#endif
}



static void php_imagickpixel_object_free_storage(IM_ZEND_OBJECT *object TSRMLS_DC)
{
	php_imagickpixel_object *intern = php_imagickpixel_fetch_object(object);

	if (!intern) {
		return;
	}
	if (!intern->initialized_via_iterator && intern->pixel_wand)
		intern->pixel_wand = DestroyPixelWand(intern->pixel_wand);

	zend_object_std_dtor(&intern->zo TSRMLS_CC);
	#if PHP_VERSION_ID < 70000
		efree(intern);
	#endif
}

#ifdef IMAGICK_WITH_KERNEL
static void php_imagickkernel_object_free_storage(IM_ZEND_OBJECT *object TSRMLS_DC)
{
	php_imagickkernel_object *intern = php_imagickkernel_fetch_object(object);

	if (!intern) {
		return;
	}

	if (intern->kernel_info != NULL) {
		// This segfaults in PHP 5.6
		//intern->kernel_info = DestroyKernelInfo(intern->kernel_info);
		intern->kernel_info = NULL;
	}

	zend_object_std_dtor(&intern->zo TSRMLS_CC);
#if PHP_VERSION_ID < 70000
	efree(intern);
#endif
}
#endif


#if PHP_VERSION_ID < 50399
# define object_properties_init(zo, class_type) { \
			zval *tmp; \
			zend_hash_copy((*zo).properties, \
							&class_type->default_properties, \
							(copy_ctor_func_t) zval_add_ref, \
							(void *) &tmp, \
							sizeof(zval *)); \
		 }
#endif

#if PHP_VERSION_ID >= 70000
static zend_object * php_imagick_object_new_ex(zend_class_entry *class_type, php_imagick_object **ptr, zend_bool init_wand TSRMLS_DC)
#else
static zend_object_value php_imagick_object_new_ex(zend_class_entry *class_type, php_imagick_object **ptr, zend_bool init_wand TSRMLS_DC)
#endif

{
	php_imagick_object *intern;

	/* Allocate memory for it */
#if PHP_VERSION_ID >= 70000
	intern = ecalloc(1,
		sizeof(php_imagick_object) +
		zend_object_properties_size(class_type));
#else
	zend_object_value retval;

	intern = (php_imagick_object *) emalloc(sizeof(php_imagick_object));
	memset(&intern->zo, 0, sizeof(zend_object));
#endif

#if defined(ZTS) && defined(PHP_WIN32)
	/* If its our thread then we already have the lock so no need to lock again */
	if (imagick_thread_id != tsrm_thread_id()) {
		tsrm_mutex_lock(imagick_mutex);
		imagick_thread_id = tsrm_thread_id();
	}
#endif

	if (ptr) {
		*ptr = intern;
	}

	/* Set the magickwand */
	if (init_wand) {
		intern->magick_wand = NewMagickWand();

		if (!intern->magick_wand) {
			zend_error(E_ERROR, "Failed to create Imagick object, could not set magick_wand");
		}
	} else {
		intern->magick_wand = NULL;
	}
	intern->next_out_of_bound = 0;
	intern->progress_monitor_name = NULL;

	zend_object_std_init(&intern->zo, class_type TSRMLS_CC);
	object_properties_init(&intern->zo, class_type);

#if PHP_VERSION_ID >= 70000
	intern->zo.handlers = &imagick_object_handlers;
	return &intern->zo;
#else
	retval.handle = zend_objects_store_put(intern, NULL, (zend_objects_free_object_storage_t) php_imagick_object_free_storage, NULL TSRMLS_CC);
    retval.handlers = (zend_object_handlers *) &imagick_object_handlers;
    return retval;
#endif
}


#if PHP_VERSION_ID >= 70000
static zend_object * php_imagick_object_new(zend_class_entry *class_type TSRMLS_DC)
#else
static zend_object_value php_imagick_object_new(zend_class_entry *class_type TSRMLS_DC)
#endif
{
	return php_imagick_object_new_ex(class_type, NULL, 1 TSRMLS_CC);
}

#if PHP_VERSION_ID >= 70000
static zend_object * php_imagickdraw_object_new_ex(zend_class_entry *class_type, php_imagickdraw_object **ptr, zend_bool init_wand TSRMLS_DC)
#else
static zend_object_value php_imagickdraw_object_new_ex(zend_class_entry *class_type, php_imagickdraw_object **ptr, zend_bool init_wand TSRMLS_DC)
#endif
{
	php_imagickdraw_object *intern;

#if PHP_VERSION_ID >= 70000
	/* Allocate memory for it */
	intern = ecalloc(1,
		sizeof(php_imagickdraw_object) +
		zend_object_properties_size(class_type));

	zend_object_std_init(&intern->zo, class_type TSRMLS_CC);
	object_properties_init(&intern->zo, class_type);
	intern->zo.handlers = &imagickdraw_object_handlers;
#else
	zend_object_value retval;
	intern = (php_imagickdraw_object *) emalloc(sizeof(php_imagickdraw_object));
	memset(&intern->zo, 0, sizeof(zend_object));
#endif

	if (ptr) {
		*ptr = intern;
	}

	/* Set the DrawingWand */
	if (init_wand) {
		intern->drawing_wand = NewDrawingWand();

		if (!intern->drawing_wand) {
			zend_error(E_ERROR, "Failed to create ImagickDraw object");
		}
	} else {
		intern->drawing_wand = NULL;
	}
#if PHP_VERSION_ID >= 70000
	intern->zo.handlers = &imagickdraw_object_handlers;

	return &intern->zo;
#else
	/* ALLOC_HASHTABLE(intern->zo.properties); */
	zend_object_std_init(&intern->zo, class_type TSRMLS_CC);
	object_properties_init(&intern->zo, class_type);
	retval.handle = zend_objects_store_put(intern, NULL, (zend_objects_free_object_storage_t) php_imagickdraw_object_free_storage, NULL TSRMLS_CC);
	retval.handlers = (zend_object_handlers *) &imagickdraw_object_handlers;

	return retval;
#endif
}

#if PHP_VERSION_ID >= 70000
static zend_object * php_imagickdraw_object_new(zend_class_entry *class_type TSRMLS_DC)
#else
static zend_object_value php_imagickdraw_object_new(zend_class_entry *class_type TSRMLS_DC)
#endif
{
	return php_imagickdraw_object_new_ex(class_type, NULL, 1 TSRMLS_CC);
}

#if PHP_VERSION_ID >= 70000
static zend_object * php_imagickpixeliterator_object_new(zend_class_entry *class_type TSRMLS_DC)
#else
static zend_object_value php_imagickpixeliterator_object_new(zend_class_entry *class_type TSRMLS_DC)
#endif
{
	php_imagickpixeliterator_object *intern;

	/* Allocate memory for it */
#if PHP_VERSION_ID >= 70000
	intern = ecalloc(1,
			sizeof(php_imagickpixeliterator_object) +
			zend_object_properties_size(class_type));
#else
	zend_object_value retval;
	intern = (php_imagickpixeliterator_object *) emalloc(sizeof(php_imagickpixeliterator_object));
	memset(&intern->zo, 0, sizeof(zend_object));
#endif

	/* We cant initialize yet */
	intern->pixel_iterator = NULL;
	intern->initialized    = 0;

#if MagickLibVersion <= 0x628
	intern->rows = 0;
	intern->iterator_position = 0;
#endif

	zend_object_std_init(&intern->zo, class_type TSRMLS_CC);
	object_properties_init(&intern->zo, class_type);

#if PHP_VERSION_ID >= 70000
	intern->zo.handlers = &imagickpixeliterator_object_handlers;

	return &intern->zo;
#else
	retval.handle = zend_objects_store_put(intern, NULL, (zend_objects_free_object_storage_t) php_imagickpixeliterator_object_free_storage, NULL TSRMLS_CC);
	retval.handlers = (zend_object_handlers *) &imagickpixeliterator_object_handlers;

	return retval;
#endif
}

#if PHP_VERSION_ID >= 70000
static zend_object * php_imagickpixel_object_new_ex(zend_class_entry *class_type, php_imagickpixel_object **ptr TSRMLS_DC)
#else
static zend_object_value php_imagickpixel_object_new_ex(zend_class_entry *class_type, php_imagickpixel_object **ptr TSRMLS_DC)
#endif
{
	php_imagickpixel_object *intern;

#if PHP_VERSION_ID >= 70000
	/* Allocate memory for it */
	intern = ecalloc(1,
		sizeof(php_imagickpixel_object) +
		zend_object_properties_size(class_type));
#else
	zend_object_value retval;
	intern = (php_imagickpixel_object *) emalloc(sizeof(php_imagickpixel_object));
	memset(&intern->zo, 0, sizeof(zend_object));
#endif

	if (ptr) {
		*ptr = intern;
	}

	/* Set the pixeliterator */
	intern->pixel_wand = NULL;
	intern->initialized_via_iterator = 0;
	zend_object_std_init(&intern->zo, class_type TSRMLS_CC);
	object_properties_init(&intern->zo, class_type);
	
#if PHP_VERSION_ID >= 70000
	intern->zo.handlers = &imagickpixel_object_handlers;

	return &intern->zo;
#else
	retval.handle = zend_objects_store_put(intern, NULL, (zend_objects_free_object_storage_t) php_imagickpixel_object_free_storage, NULL TSRMLS_CC);
	retval.handlers = (zend_object_handlers *) &imagickpixel_object_handlers;

	return retval;
#endif

}

#if PHP_VERSION_ID >= 70000
static zend_object * php_imagickpixel_object_new(zend_class_entry *class_type TSRMLS_DC)
#else
static zend_object_value php_imagickpixel_object_new(zend_class_entry *class_type TSRMLS_DC)
#endif
{
	return php_imagickpixel_object_new_ex(class_type, NULL TSRMLS_CC);
}

#ifdef IMAGICK_WITH_KERNEL

#if PHP_VERSION_ID >= 70000
static zend_object *  php_imagickkernel_object_new_ex(zend_class_entry *class_type, php_imagickkernel_object **ptr TSRMLS_DC)
#else
static zend_object_value php_imagickkernel_object_new_ex(zend_class_entry *class_type, php_imagickkernel_object **ptr TSRMLS_DC)
#endif
{

	php_imagickkernel_object *intern;

	/* Allocate memory for it */
#if PHP_VERSION_ID >= 70000
	intern = ecalloc(1,
		sizeof(php_imagickkernel_object) +
		zend_object_properties_size(class_type));
#else
	zend_object_value retval;
	intern = (php_imagickkernel_object *) emalloc(sizeof(php_imagickkernel_object));
		memset(&intern->zo, 0, sizeof(zend_object));
#endif

	if (ptr) {
		*ptr = intern;
	}

	/* Set the kernel */
	intern->kernel_info = NULL;

	/* ALLOC_HASHTABLE(intern->zo.properties); */
	zend_object_std_init(&intern->zo, class_type TSRMLS_CC);
	object_properties_init(&intern->zo, class_type);

#if PHP_VERSION_ID >= 70000
	intern->zo.handlers = &imagickkernel_object_handlers;

	return &intern->zo;
#else
	retval.handle = zend_objects_store_put(intern, NULL, (zend_objects_free_object_storage_t) php_imagickkernel_object_free_storage, NULL TSRMLS_CC);
	retval.handlers = (zend_object_handlers *) &imagickkernel_object_handlers;

	return retval;
#endif
}
#endif

#undef object_properties_init

#ifdef IMAGICK_WITH_KERNEL
#if PHP_VERSION_ID >= 70000
static zend_object * php_imagickkernel_object_new(zend_class_entry *class_type TSRMLS_DC)
#else
static zend_object_value php_imagickkernel_object_new(zend_class_entry *class_type TSRMLS_DC)
#endif
{
	return php_imagickkernel_object_new_ex(class_type, NULL TSRMLS_CC);
}
#endif



PHP_INI_BEGIN()
	STD_PHP_INI_ENTRY("imagick.locale_fix", "0", PHP_INI_ALL, OnUpdateBool, locale_fix, zend_imagick_globals, imagick_globals)
	STD_PHP_INI_ENTRY("imagick.skip_version_check", "0", PHP_INI_ALL, OnUpdateBool, skip_version_check, zend_imagick_globals, imagick_globals)
	STD_PHP_INI_ENTRY("imagick.progress_monitor", "0", PHP_INI_SYSTEM, OnUpdateBool, progress_monitor, zend_imagick_globals, imagick_globals)

	STD_PHP_INI_ENTRY("imagick.set_single_thread", "1", PHP_INI_SYSTEM, OnUpdateBool, set_single_thread, zend_imagick_globals, imagick_globals)
	STD_PHP_INI_ENTRY("imagick.shutdown_sleep_count",  "10", PHP_INI_ALL, OnUpdateLong, shutdown_sleep_count, zend_imagick_globals, imagick_globals)
	STD_PHP_INI_ENTRY("imagick.allow_zero_dimension_images", "0", PHP_INI_ALL, OnUpdateBool, allow_zero_dimension_images, zend_imagick_globals, imagick_globals)
PHP_INI_END()

static void php_imagick_init_globals(zend_imagick_globals *imagick_globals)
{
	imagick_globals->locale_fix = 0;
	imagick_globals->progress_monitor = 0;
	imagick_globals->skip_version_check = 0;
	imagick_globals->set_single_thread = 1;
	imagick_globals->allow_zero_dimension_images = 0;
	// 10 is magick number, that seems to be enough.
	imagick_globals->shutdown_sleep_count = 10;
}


#if PHP_VERSION_ID >= 80000
static int php_imagick_count_elements(zend_object *object, im_long *count) /* {{{ */
{
	php_imagick_object *intern= php_imagick_fetch_object(object);
#else
static int php_imagick_count_elements(zval *object, im_long *count TSRMLS_DC) /* {{{ */
{
	php_imagick_object *intern= Z_IMAGICK_P(object);
#endif

	if (intern->magick_wand) {
		*count = MagickGetNumberImages(intern->magick_wand);
		return SUCCESS;
	}
	return FAILURE;
}


#if PHP_VERSION_ID >= 80000
static zval *php_imagick_read_property(zend_object *object, zend_string *member, int type, void **cache_slot, zval *rv)
{
	int ret;
	php_imagick_object *intern;
	zval *retval = NULL;
    const zend_object_handlers *std_hnd;


	std_hnd = zend_get_std_object_handlers();

	ret = std_hnd->has_property(object, member, type, cache_slot TSRMLS_CC);

	if (ret) {
		//TODO - this would allow better immutability
		//ZVAL_COPY_VALUE(retval, std_hnd->read_property(object, member, type, cache_slot, rv TSRMLS_CC));
		retval = std_hnd->read_property(object, member, type, cache_slot, rv TSRMLS_CC);
	}
	else {

		intern = php_imagick_fetch_object(object);
		/* Do we have any images? */
		if (MagickGetNumberImages(intern->magick_wand)) {

			//TODO - this seems redundant
			/* Is this overloaded? */
			if (!strcmp(ZSTR_VAL(member), "width") ||
				!strcmp(ZSTR_VAL(member), "height") ||
				!strcmp(ZSTR_VAL(member), "format")) {

				if (!strcmp(ZSTR_VAL(member), "width")) {
					retval = rv;
					ZVAL_LONG(retval, MagickGetImageWidth(intern->magick_wand));
				} else if (!strcmp(ZSTR_VAL(member), "height")) {
					retval = rv;
					ZVAL_LONG(retval, MagickGetImageHeight(intern->magick_wand));
				} else if (!strcmp(ZSTR_VAL(member), "format")) {
					char *format = MagickGetImageFormat(intern->magick_wand);

					if (format) {
						retval = rv;
						ZVAL_STRING(retval, format);
						php_strtolower(Z_STRVAL_P(retval), Z_STRLEN_P(retval));
						IMAGICK_FREE_MAGICK_MEMORY(format);
					} else {
						retval = rv;
						ZVAL_STRING(retval, "");
					}
				}
			}
		}
	}

	if (!retval) {
		//TODO - why is the error silent - it would be a small BC break
		//to enable the warning. I think it would be the correct thing to do though.
		//zend_error(E_NOTICE,"Undefined property: \Imagick::$%s", Z_STRVAL_P(member));
		retval = &EG(uninitialized_zval);
	}

	return retval;
}

#else // PHP_VERSION_ID >= 80000

#if PHP_VERSION_ID >= 70000
static zval *php_imagick_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv TSRMLS_DC)
{
	int ret;
	php_imagick_object *intern;
	zval *retval = NULL;
	zval tmp_member;
    const zend_object_handlers *std_hnd;

	ZVAL_DEREF(member);
	if (Z_TYPE_P(member) != IS_STRING) {
		tmp_member = *member;
		zval_copy_ctor(&tmp_member);
		convert_to_string(&tmp_member);
		member = &tmp_member;
    }

	std_hnd = zend_get_std_object_handlers();

	ret = std_hnd->has_property(object, member, type, cache_slot TSRMLS_CC);

	if (ret) {
		//TODO - this would allow better immutability
		//ZVAL_COPY_VALUE(retval, std_hnd->read_property(object, member, type, cache_slot, rv TSRMLS_CC));
		retval = std_hnd->read_property(object, member, type, cache_slot, rv TSRMLS_CC);
	}
	else {

		intern = Z_IMAGICK_P(object);
		/* Do we have any images? */
		if (MagickGetNumberImages(intern->magick_wand)) {

			//TODO - this seems redundant
			/* Is this overloaded? */
			if (!strcmp(Z_STRVAL_P(member), "width") ||
				!strcmp(Z_STRVAL_P(member), "height") ||
				!strcmp(Z_STRVAL_P(member), "format")) {

				if (!strcmp(Z_STRVAL_P(member), "width")) {
					retval = rv;
					ZVAL_LONG(retval, MagickGetImageWidth(intern->magick_wand));
				} else if (!strcmp(Z_STRVAL_P(member), "height")) {
					retval = rv;
					ZVAL_LONG(retval, MagickGetImageHeight(intern->magick_wand));
				} else if (!strcmp(Z_STRVAL_P(member), "format")) {
					char *format = MagickGetImageFormat(intern->magick_wand);

					if (format) {
						retval = rv;
						ZVAL_STRING(retval, format);
						php_strtolower(Z_STRVAL_P(retval), Z_STRLEN_P(retval));
						IMAGICK_FREE_MAGICK_MEMORY(format);
					} else {
						retval = rv;
						ZVAL_STRING(retval, "");
					}
				}
			}
		}
	}

	if (!retval) {
		//TODO - why is the error silent - it would be a small BC break
		//to enable the warning. I think it would be the correct thing to do though.
		//zend_error(E_NOTICE,"Undefined property: \Imagick::$%s", Z_STRVAL_P(member));
		retval = &EG(uninitialized_zval);
	}

	if (member == &tmp_member) {
		zval_dtor(member);
	}

	return retval;
}

#else

#if PHP_VERSION_ID < 50399
static zval *php_imagick_read_property(zval *object, zval *member, int type TSRMLS_DC)
#else
static zval *php_imagick_read_property(zval *object, zval *member, int type, const zend_literal *key TSRMLS_DC)
#endif
{
	int ret;
	php_imagick_object *intern;
	zval *retval = NULL, tmp_member;
    zend_object_handlers *std_hnd;

	if (Z_TYPE_P(member) != IS_STRING) {
		tmp_member = *member;
		zval_copy_ctor(&tmp_member);
		convert_to_string(&tmp_member);
		member = &tmp_member;
    }

	std_hnd = zend_get_std_object_handlers();
#if PHP_VERSION_ID < 50399
	ret = std_hnd->has_property(object, member, 2 TSRMLS_CC);
#else
	ret = std_hnd->has_property(object, member, 2, key TSRMLS_CC);
#endif	

	if (ret) {
		std_hnd = zend_get_std_object_handlers();
#if PHP_VERSION_ID < 50399
		retval = std_hnd->read_property(object, member, type TSRMLS_CC);
#else
		retval = std_hnd->read_property(object, member, type, key TSRMLS_CC);
#endif
	} else {
		intern = (php_imagick_object *) zend_object_store_get_object(object TSRMLS_CC);
		/* Do we have any images? */
		if (MagickGetNumberImages(intern->magick_wand)) {
			/* Is this overloaded? */
			if (!strcmp(Z_STRVAL_P(member), "width") ||
				!strcmp(Z_STRVAL_P(member), "height") ||
				!strcmp(Z_STRVAL_P(member), "format")) {

				MAKE_STD_ZVAL(retval);
#ifdef Z_SET_REFCOUNT_P
				Z_SET_REFCOUNT_P(retval, 0);
#else
				retval->refcount = 0;
#endif
				if (!strcmp(Z_STRVAL_P(member), "width")) {
					ZVAL_LONG(retval, MagickGetImageWidth(intern->magick_wand));
				} else if (!strcmp(Z_STRVAL_P(member), "height")) {
					ZVAL_LONG(retval, MagickGetImageHeight(intern->magick_wand));
				} else if (!strcmp(Z_STRVAL_P(member), "format")) {
					char *format = MagickGetImageFormat(intern->magick_wand);

					if (format) {
						ZVAL_STRING(retval, format, 1);
						php_strtolower(Z_STRVAL_P(retval), Z_STRLEN_P(retval));
						IMAGICK_FREE_MAGICK_MEMORY(format);
					} else {
						ZVAL_STRING(retval, "", 1);
					}
				}
			}
		}
	}
	if (member == &tmp_member) {
    	zval_dtor(member);
    }

	if (!retval) {
		retval = EG(uninitialized_zval_ptr);
	}
	return retval;
}
#endif

#endif

#if PHP_VERSION_ID >= 80000
static zend_object * php_imagick_clone_imagick_object(zend_object *this_ptr)
#elif PHP_VERSION_ID >= 70000
static zend_object * php_imagick_clone_imagick_object(zval *this_ptr TSRMLS_DC)
#else
static zend_object_value php_imagick_clone_imagick_object(zval *this_ptr TSRMLS_DC)
#endif
{
	MagickWand *wand_copy = NULL;
	php_imagick_object *new_obj = NULL;
#if PHP_VERSION_ID >= 80000
	php_imagick_object *old_obj = php_imagick_fetch_object(this_ptr);
	zend_object * new_zo = php_imagick_object_new_ex(old_obj->zo.ce, &new_obj, 0 TSRMLS_CC);
	zend_objects_clone_members(&new_obj->zo, &old_obj->zo TSRMLS_CC);
#elif PHP_VERSION_ID >= 70000
	php_imagick_object *old_obj = Z_IMAGICK_P(this_ptr);
	zend_object * new_zo = php_imagick_object_new_ex(old_obj->zo.ce, &new_obj, 0 TSRMLS_CC);
	zend_objects_clone_members(&new_obj->zo, &old_obj->zo TSRMLS_CC);
#else
	php_imagick_object *old_obj = (php_imagick_object *) zend_object_store_get_object(this_ptr TSRMLS_CC);
	zend_object_value new_zo = php_imagick_object_new_ex(old_obj->zo.ce, &new_obj, 0 TSRMLS_CC);
	zend_objects_clone_members(&new_obj->zo, new_zo, &old_obj->zo, Z_OBJ_HANDLE_P(this_ptr) TSRMLS_CC);
#endif

	wand_copy = CloneMagickWand(old_obj->magick_wand);
	if (!wand_copy) {
		zend_error(E_ERROR, "Failed to clone Imagick object");
	} else {
		php_imagick_replace_magickwand(new_obj, wand_copy);
		new_obj->next_out_of_bound = old_obj->next_out_of_bound;

		if (old_obj->progress_monitor_name) {
			new_obj->progress_monitor_name = estrdup(old_obj->progress_monitor_name);
		}
	}
	return new_zo;
}

#if PHP_VERSION_ID >= 80000
static zend_object * php_imagick_clone_imagickdraw_object(zend_object *this_ptr)
#elif PHP_VERSION_ID >= 70000
static zend_object * php_imagick_clone_imagickdraw_object(zval *this_ptr TSRMLS_DC)
#else
static zend_object_value php_imagick_clone_imagickdraw_object(zval *this_ptr TSRMLS_DC)
#endif
{
	DrawingWand *wand_copy = NULL;
	php_imagickdraw_object *new_obj = NULL;
#if PHP_VERSION_ID >= 80000
	php_imagickdraw_object *old_obj = php_imagickdraw_fetch_object(this_ptr);
	zend_object * new_zo = php_imagickdraw_object_new_ex(old_obj->zo.ce, &new_obj, 0 TSRMLS_CC);
	zend_objects_clone_members(&new_obj->zo, &old_obj->zo TSRMLS_CC);
#elif PHP_VERSION_ID >= 70000
	php_imagickdraw_object *old_obj = Z_IMAGICKDRAW_P(this_ptr);
	zend_object * new_zo = php_imagickdraw_object_new_ex(old_obj->zo.ce, &new_obj, 0 TSRMLS_CC);
	zend_objects_clone_members(&new_obj->zo, &old_obj->zo TSRMLS_CC);
#else
	php_imagickdraw_object *old_obj = (php_imagickdraw_object *) zend_object_store_get_object(this_ptr TSRMLS_CC);
	zend_object_value new_zo = php_imagickdraw_object_new_ex(old_obj->zo.ce, &new_obj, 0 TSRMLS_CC);
	zend_objects_clone_members(&new_obj->zo, new_zo, &old_obj->zo, Z_OBJ_HANDLE_P(this_ptr) TSRMLS_CC);
#endif

	wand_copy = CloneDrawingWand(old_obj->drawing_wand);

	if (!wand_copy) {
		zend_error(E_ERROR, "Failed to clone ImagickDraw object");
	} else {
		php_imagick_replace_drawingwand(new_obj, wand_copy);
	}
	return new_zo;
}


#if PHP_VERSION_ID >= 80000
static zend_object * php_imagick_clone_imagickpixel_object(zend_object *this_ptr)
#elif PHP_VERSION_ID >= 70000
static zend_object * php_imagick_clone_imagickpixel_object(zval *this_ptr TSRMLS_DC)
#else
static zend_object_value php_imagick_clone_imagickpixel_object(zval *this_ptr TSRMLS_DC)
#endif
{
	PixelWand *wand_copy = NULL;
	php_imagickpixel_object *new_obj = NULL;

#if PHP_VERSION_ID >= 80000
	zend_object *new_zo;
	php_imagickpixel_object *old_obj = php_imagickpixel_fetch_object(this_ptr);
	new_zo = php_imagickpixel_object_new_ex(old_obj->zo.ce, &new_obj TSRMLS_CC);
	zend_objects_clone_members(&new_obj->zo, &old_obj->zo TSRMLS_CC);
#elif PHP_VERSION_ID >= 70000
	zend_object *new_zo;
	php_imagickpixel_object *old_obj = Z_IMAGICKPIXEL_P(this_ptr);
	new_zo = php_imagickpixel_object_new_ex(old_obj->zo.ce, &new_obj TSRMLS_CC);
	zend_objects_clone_members(&new_obj->zo, &old_obj->zo TSRMLS_CC);
#else
	php_imagickpixel_object *old_obj = (php_imagickpixel_object *) zend_object_store_get_object(this_ptr TSRMLS_CC);
	zend_object_value new_zo = php_imagickpixel_object_new_ex(old_obj->zo.ce, &new_obj TSRMLS_CC);
	zend_objects_clone_members(&new_obj->zo, new_zo, &old_obj->zo, Z_OBJ_HANDLE_P(this_ptr) TSRMLS_CC);
#endif

	wand_copy = php_imagick_clone_pixelwand (old_obj->pixel_wand);
	if (!wand_copy) {
		zend_error(E_ERROR, "Failed to clone ImagickPixel object");
	} else {
		php_imagick_replace_pixelwand(new_obj, wand_copy);
		new_obj->initialized_via_iterator = 0;
	}
	return new_zo;
}

#ifdef IMAGICK_WITH_KERNEL

#if PHP_VERSION_ID >= 80000
static zend_object * php_imagick_clone_imagickkernel_object(zend_object *this_ptr TSRMLS_DC)
#elif PHP_VERSION_ID >= 70000
static zend_object * php_imagick_clone_imagickkernel_object(zval *this_ptr TSRMLS_DC)
#else
static zend_object_value php_imagick_clone_imagickkernel_object(zval *this_ptr TSRMLS_DC)
#endif
{
	KernelInfo *kernel_info_copy = NULL;
	php_imagickkernel_object *new_obj = NULL;

#if PHP_VERSION_ID >= 80000
	zend_object *new_zo;
	php_imagickkernel_object *old_obj = php_imagickkernel_fetch_object(this_ptr);
	new_zo = php_imagickkernel_object_new_ex(old_obj->zo.ce, &new_obj TSRMLS_CC);
	zend_objects_clone_members(&new_obj->zo, &old_obj->zo TSRMLS_CC);
#elif PHP_VERSION_ID >= 70000
	zend_object *new_zo;
	php_imagickkernel_object *old_obj = Z_IMAGICKKERNEL_P(this_ptr);
	new_zo = php_imagickkernel_object_new_ex(old_obj->zo.ce, &new_obj TSRMLS_CC);
	zend_objects_clone_members(&new_obj->zo, &old_obj->zo TSRMLS_CC);
#else
	zend_object_value new_zo;
	php_imagickkernel_object *old_obj = (php_imagickkernel_object *) zend_object_store_get_object(this_ptr TSRMLS_CC);
	new_zo = php_imagickkernel_object_new_ex(old_obj->zo.ce, &new_obj TSRMLS_CC);
	zend_objects_clone_members(&new_obj->zo, new_zo, &old_obj->zo, Z_OBJ_HANDLE_P(this_ptr) TSRMLS_CC);
#endif

	if (old_obj->kernel_info != NULL) {
		kernel_info_copy = CloneKernelInfo(old_obj->kernel_info);

		if (!kernel_info_copy) {
			zend_error(E_ERROR, "Failed to clone ImagickKernel object");
		}
		else {
			new_obj->kernel_info = kernel_info_copy;
		}
	}

	return new_zo;
}
#endif


static void checkImagickVersion()
{
	//This gets the version that Imagick was compiled against.
	size_t imagickVersion = MagickLibVersion;

	//This gets the version of ImageMagick that has been loaded
	size_t imageMagickLibraryVersion;

	GetMagickVersion(&imageMagickLibraryVersion);

	if (imagickVersion == imageMagickLibraryVersion) {
		return;
	}

	zend_error(
		E_WARNING,
		"Version warning: Imagick was compiled against ImageMagick version %lu but version %lu is loaded. Imagick will run but may behave surprisingly",
		(unsigned long)imagickVersion,
		(unsigned long)imageMagickLibraryVersion
	);
}

PHP_MINIT_FUNCTION(imagick)
{
	zend_class_entry ce;

	/* Initialize globals */
	ZEND_INIT_MODULE_GLOBALS(imagick, php_imagick_init_globals, NULL);


	// This suppresses an 'unused parameter' warning.
	(void)type;

	/*
		Allocate some memory
	*/
	memcpy(&imagick_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	memcpy(&imagickdraw_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	memcpy(&imagickpixeliterator_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	memcpy(&imagickpixel_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
#ifdef IMAGICK_WITH_KERNEL
	memcpy(&imagickkernel_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
#endif

	/* Set custom allocators */
	MagickWandGenesis();

	/*
		Initialize exceptions (Imagick exception)
	*/
	INIT_CLASS_ENTRY(ce, PHP_IMAGICK_EXCEPTION_SC_NAME, NULL);
#if PHP_VERSION_ID >= 70000
	php_imagick_exception_class_entry = zend_register_internal_class_ex(&ce, zend_ce_exception);
#else
	php_imagick_exception_class_entry = zend_register_internal_class_ex(&ce, zend_exception_get_default(TSRMLS_C), NULL TSRMLS_CC);
#endif

	/*
	Initialize exceptions (ImagickDraw exception)
	*/
	INIT_CLASS_ENTRY(ce, PHP_IMAGICKDRAW_EXCEPTION_SC_NAME, NULL);
#if PHP_VERSION_ID >= 70000
	php_imagickdraw_exception_class_entry = zend_register_internal_class_ex(&ce, zend_ce_exception);
#else
	php_imagickdraw_exception_class_entry = zend_register_internal_class_ex(&ce, zend_exception_get_default(TSRMLS_C), NULL TSRMLS_CC);
#endif

	/*
	Initialize exceptions (ImagickPixelIterator exception)
	*/
	INIT_CLASS_ENTRY(ce, PHP_IMAGICKPIXELITERATOR_EXCEPTION_SC_NAME, NULL);
#if PHP_VERSION_ID >= 70000
	php_imagickpixeliterator_exception_class_entry = zend_register_internal_class_ex(&ce, zend_ce_exception);
#else
	php_imagickpixeliterator_exception_class_entry = zend_register_internal_class_ex(&ce, zend_exception_get_default(TSRMLS_C), NULL TSRMLS_CC);
#endif

	/*
	Initialize exceptions (ImagickPixel exception)
	*/
	INIT_CLASS_ENTRY(ce, PHP_IMAGICKPIXEL_EXCEPTION_SC_NAME, NULL);
#if PHP_VERSION_ID >= 70000
	php_imagickpixel_exception_class_entry = zend_register_internal_class_ex(&ce, zend_ce_exception);
#else
	php_imagickpixel_exception_class_entry = zend_register_internal_class_ex(&ce, zend_exception_get_default(TSRMLS_C), NULL TSRMLS_CC);
#endif

#ifdef IMAGICK_WITH_KERNEL
	/*
	Initialize exceptions (ImagickKernel exception)
	*/
	INIT_CLASS_ENTRY(ce, PHP_IMAGICKKERNEL_EXCEPTION_SC_NAME, NULL);
	#if PHP_VERSION_ID >= 70000
	php_imagickkernel_exception_class_entry = zend_register_internal_class_ex(&ce, zend_ce_exception);
    #else
	php_imagickkernel_exception_class_entry = zend_register_internal_class_ex(&ce, zend_exception_get_default(TSRMLS_C), NULL TSRMLS_CC);
    #endif

#endif

	/*
		Initialize the class (Imagick)
	*/
	INIT_CLASS_ENTRY(ce, PHP_IMAGICK_SC_NAME, php_imagick_class_methods);
	ce.create_object = php_imagick_object_new;
	imagick_object_handlers.clone_obj = php_imagick_clone_imagick_object;
	imagick_object_handlers.read_property = php_imagick_read_property;
	imagick_object_handlers.count_elements = php_imagick_count_elements;
#if PHP_VERSION_ID >= 70000
	imagick_object_handlers.offset = XtOffsetOf(php_imagick_object, zo);
	imagick_object_handlers.free_obj = php_imagick_object_free_storage;
#endif

	php_imagick_sc_entry = zend_register_internal_class(&ce TSRMLS_CC);
	zend_class_implements(php_imagick_sc_entry TSRMLS_CC, 2, zend_ce_iterator, im_ce_countable);

	/*
		Initialize the class (ImagickDraw)
	*/
	INIT_CLASS_ENTRY(ce, PHP_IMAGICKDRAW_SC_NAME, php_imagickdraw_class_methods);
	ce.create_object = php_imagickdraw_object_new;
	imagickdraw_object_handlers.clone_obj = php_imagick_clone_imagickdraw_object;
#if PHP_VERSION_ID >= 70000
	imagickdraw_object_handlers.offset = XtOffsetOf(php_imagickdraw_object, zo);
	imagickdraw_object_handlers.free_obj = php_imagickdraw_object_free_storage;
#endif
	php_imagickdraw_sc_entry = zend_register_internal_class(&ce TSRMLS_CC);

	/*
		Initialize the class (ImagickPixelIterator)
	*/
	INIT_CLASS_ENTRY(ce, PHP_IMAGICKPIXELITERATOR_SC_NAME, php_imagickpixeliterator_class_methods);
	ce.create_object = php_imagickpixeliterator_object_new;
	imagickpixeliterator_object_handlers.clone_obj = NULL;
#if PHP_VERSION_ID >= 70000
	imagickpixeliterator_object_handlers.offset = XtOffsetOf(php_imagickpixeliterator_object, zo);
	imagickpixeliterator_object_handlers.free_obj = php_imagickpixeliterator_object_free_storage;
#endif
	php_imagickpixeliterator_sc_entry = zend_register_internal_class(&ce TSRMLS_CC);
	zend_class_implements(php_imagickpixeliterator_sc_entry TSRMLS_CC, 1, zend_ce_iterator);

	/*
		Initialize the class (ImagickPixel)
	*/
	INIT_CLASS_ENTRY(ce, PHP_IMAGICKPIXEL_SC_NAME, php_imagickpixel_class_methods);
	ce.create_object = php_imagickpixel_object_new;
	imagickpixel_object_handlers.clone_obj = php_imagick_clone_imagickpixel_object;
#if PHP_VERSION_ID >= 70000
	imagickpixel_object_handlers.offset = XtOffsetOf(php_imagickpixel_object, zo);
	imagickpixel_object_handlers.free_obj = php_imagickpixel_object_free_storage;
#endif

	php_imagickpixel_sc_entry = zend_register_internal_class(&ce TSRMLS_CC);

#ifdef IMAGICK_WITH_KERNEL
	/*
		Initialize the class (ImagickKernel)
	*/
	INIT_CLASS_ENTRY(ce, PHP_IMAGICKKERNEL_SC_NAME, php_imagickkernel_class_methods);
	ce.create_object = php_imagickkernel_object_new;
	// Disabled until can be compiled under wall correctly
	imagickkernel_object_handlers.get_debug_info = php_imagickkernel_get_debug_info;
	imagickkernel_object_handlers.clone_obj = php_imagick_clone_imagickkernel_object;
	#if PHP_VERSION_ID >= 70000
	imagickkernel_object_handlers.offset = XtOffsetOf(php_imagickkernel_object, zo);
	imagickkernel_object_handlers.free_obj = php_imagickkernel_object_free_storage;
    #endif

	php_imagickkernel_sc_entry = zend_register_internal_class(&ce TSRMLS_CC);
#endif

	php_imagick_initialize_constants (TSRMLS_C);

#if defined(ZTS) && defined(PHP_WIN32)
	imagick_mutex = tsrm_mutex_alloc();
#endif

	REGISTER_INI_ENTRIES();

	if (!IMAGICK_G(skip_version_check)) {
		checkImagickVersion();
	}

	return SUCCESS;
}


PHP_MINFO_FUNCTION(imagick)
{

#if PHP_VERSION_ID >= 70000
	smart_string formats = {0};
#else
	smart_str formats = {0};
#endif

	char **supported_formats, *buffer;
	unsigned long i;
	size_t num_formats;
	size_t version_number;

	num_formats = 0;

	supported_formats = MagickQueryFormats("*", &num_formats);
	spprintf(&buffer, 0, "%ld", (long)num_formats);

	php_info_print_table_start();
	php_info_print_table_header(2, "imagick module", "enabled");
	php_info_print_table_row(2, "imagick module version", PHP_IMAGICK_VERSION);
#ifdef IMAGICK_WITH_KERNEL
	php_info_print_table_row(2, "imagick classes", "Imagick, ImagickDraw, ImagickPixel, ImagickPixelIterator, ImagickKernel");
#else
	php_info_print_table_row(2, "imagick classes", "Imagick, ImagickDraw, ImagickPixel, ImagickPixelIterator");
#endif
#ifdef MagickVersion
	php_info_print_table_row(2, "Imagick compiled with ImageMagick version", MagickVersion);
#endif
	php_info_print_table_row(2, "Imagick using ImageMagick library version", MagickGetVersion(&version_number));
	php_info_print_table_row(2, "ImageMagick copyright", MagickGetCopyright());
	php_info_print_table_row(2, "ImageMagick release date", MagickGetReleaseDate());
	php_info_print_table_row(2, "ImageMagick number of supported formats: ", buffer);

	efree (buffer);

	if (supported_formats) {
		for (i = 0; i < num_formats; i++) {
#if PHP_VERSION_ID >= 70000
			if (i != 0) {
 				smart_string_appends(&formats, ", ");
			}
			smart_string_appends(&formats, supported_formats[i]);
#else
			smart_str_appends(&formats, supported_formats[i]);
			if (i != (num_formats - 1)) {
				smart_str_appends(&formats, ", ");
			}
#endif
			IMAGICK_FREE_MAGICK_MEMORY(supported_formats[i]);
		}
		
#if PHP_VERSION_ID >= 70000
		smart_string_0(&formats);
		php_info_print_table_row(2, "ImageMagick supported formats", formats.c);
		smart_string_free(&formats);
#else
		smart_str_0(&formats);
		php_info_print_table_row(2, "ImageMagick supported formats", formats.c);
		smart_str_free(&formats);
#endif
		IMAGICK_FREE_MAGICK_MEMORY(supported_formats);
	}

	php_info_print_table_end();
	DISPLAY_INI_ENTRIES();
}


PHP_MSHUTDOWN_FUNCTION(imagick)
{
#if !HAVE_OMP_PAUSE_RESOURCE_ALL
    int i;
#endif

// This suppresses an 'unused parameter' warning.
	(void)type;

	MagickWandTerminus();

#if HAVE_OMP_PAUSE_RESOURCE_ALL
	omp_pause_resource_all(omp_pause_hard);
#else
    // Sleep for a bit to hopefully allow OpenMP to
    // shut down the threads it created, and avoid a segfault
    // This hack won't be needed once everyone is compiling ImageMagick
    // against a version of OpenMP that has omp_pause_resource_all()
	for (i = 0; i < 100 && i < IMAGICK_G(shutdown_sleep_count); i += 1) {
		usleep(1000);
	}
#endif

#if defined(ZTS) && defined(PHP_WIN32)
	tsrm_mutex_free(imagick_mutex);
#endif
	UNREGISTER_INI_ENTRIES();
	return SUCCESS;
}

PHP_RINIT_FUNCTION(imagick)
{
	// This suppresses an 'unused parameter' warning.
	(void)type;
	(void)module_number;

	IMAGICK_G(progress_callback) = NULL;

	if (IMAGICK_G(set_single_thread)) {
		MagickSetResourceLimit(ThreadResource, 1);
	}

	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(imagick)
{
	php_imagick_callback* progress_callback = IMAGICK_G(progress_callback);
	// This suppresses an 'unused parameter' warning.
	(void)type;
	(void)module_number;

	if (progress_callback) {
		php_imagick_cleanup_progress_callback(progress_callback TSRMLS_CC);
		efree(progress_callback);
		IMAGICK_G(progress_callback) = NULL;
	}

#if defined(ZTS) && defined(PHP_WIN32)
	/* We have the lock so lets release it */
	if (imagick_thread_id == tsrm_thread_id()) {
		imagick_thread_id = (THREAD_T)NULL;
		tsrm_mutex_unlock(imagick_mutex);
	}
#endif
	return SUCCESS;
}

#if ZEND_MODULE_API_NO >= 20050922
static const zend_module_dep imagick_deps[] = {
    ZEND_MOD_CONFLICTS("gmagick")
    ZEND_MOD_REQUIRED("standard")
	ZEND_MOD_REQUIRED("spl")
	ZEND_MOD_END
};
#endif

zend_module_entry imagick_module_entry =
{
#if ZEND_MODULE_API_NO >= 20050922
        STANDARD_MODULE_HEADER_EX, NULL,
        imagick_deps,
#else
        STANDARD_MODULE_HEADER,
#endif
        PHP_IMAGICK_EXTNAME,
        php_imagick_functions,                  /* Functions */
        PHP_MINIT(imagick),                     /* MINIT */
        PHP_MSHUTDOWN(imagick),                 /* MSHUTDOWN */
        PHP_RINIT(imagick),                     /* RINIT */
        PHP_RSHUTDOWN(imagick),
        PHP_MINFO(imagick),                     /* MINFO */
        PHP_IMAGICK_VERSION,
        STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_IMAGICK
ZEND_GET_MODULE(imagick)
#endif

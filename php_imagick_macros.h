/*
   +----------------------------------------------------------------------+
   | PHP Version 5 / Imagick	                                          |
   +----------------------------------------------------------------------+
   | Copyright (c) 2006-2009 Mikko Koppanen, Scott MacVicar               |
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

/* Define a set of macros to throw exceptions */
#define IMAGICK_THROW_EXCEPTION_WITH_MESSAGE(type, description, code) \
{ \
	switch(type) { \
		case 1: \
			zend_throw_exception(php_imagick_exception_class_entry, description, (long)code TSRMLS_CC); \
			RETURN_NULL(); \
		break; \
		case 2: \
			zend_throw_exception(php_imagickdraw_exception_class_entry, description, (long)code TSRMLS_CC); \
			RETURN_NULL(); \
		break; \
		case 3: \
			zend_throw_exception(php_imagickpixeliterator_exception_class_entry, description, (long)code TSRMLS_CC); \
			RETURN_NULL(); \
		break; \
		case 4: \
			zend_throw_exception(php_imagickpixel_exception_class_entry, description, (long)code TSRMLS_CC); \
			RETURN_NULL(); \
		break; \
	} \
} \

#define IMAGICK_THROW_IMAGICK_EXCEPTION(magick_wand, fallback, code) \
{ \
	ExceptionType severity; \
	char *description; \
	description = MagickGetException(magick_wand, &severity); \
	if (strlen(description) == 0) { \
		IMAGICK_FREE_MEMORY(char *, description); \
	} \
	if (!description) { \
		zend_throw_exception(php_imagick_exception_class_entry, fallback, (long)code TSRMLS_CC); \
		RETURN_NULL(); \
	} else { \
		zend_throw_exception(php_imagick_exception_class_entry, description, (long)severity TSRMLS_CC); \
		IMAGICK_FREE_MEMORY(char *, description); \
		MagickClearException(magick_wand); \
		RETURN_NULL(); \
	} \
} \

#define IMAGICK_THROW_IMAGICKDRAW_EXCEPTION(drawing_wand, fallback, code) \
{ \
	ExceptionType severity; \
	char *description; \
	description = DrawGetException(drawing_wand, &severity); \
	if (strlen(description) == 0) { \
		IMAGICK_FREE_MEMORY(char *, description); \
	} \
	if (!description) { \
		zend_throw_exception(php_imagickdraw_exception_class_entry, fallback, (long)code TSRMLS_CC); \
		RETURN_NULL(); \
	} else { \
		zend_throw_exception(php_imagickdraw_exception_class_entry, description, (long)severity TSRMLS_CC); \
		IMAGICK_FREE_MEMORY(char *, description); \
		DrawClearException(drawing_wand); \
		RETURN_NULL(); \
	} \
} \

#define IMAGICK_THROW_IMAGICKPIXEL_EXCEPTION(pixel_wand, fallback, code) \
{ \
	ExceptionType severity; \
	char *description; \
	description = PixelGetException(pixel_wand, &severity); \
	if (strlen(description) == 0) { \
		IMAGICK_FREE_MEMORY(char *, description); \
	} \
	if (!description) { \
		zend_throw_exception(php_imagickpixel_exception_class_entry, fallback, (long)code TSRMLS_CC); \
		RETURN_NULL(); \
	} else { \
		zend_throw_exception(php_imagickpixel_exception_class_entry, description, (long)severity TSRMLS_CC); \
		IMAGICK_FREE_MEMORY(char *, description); \
		PixelClearException(pixel_wand); \
		RETURN_NULL(); \
	} \
} \

#define IMAGICK_THROW_IMAGICKPIXELITERATOR_EXCEPTION(pixel_iterator, fallback, code) \
{ \
	ExceptionType severity; \
	char *description; \
	description = PixelGetIteratorException(pixel_iterator, &severity); \
	if (strlen(description) == 0) { \
		IMAGICK_FREE_MEMORY(char *, description); \
	} \
	if (!description) { \
		zend_throw_exception(php_imagickpixeliterator_exception_class_entry, fallback, (long)code TSRMLS_CC); \
		RETURN_NULL(); \
	} else { \
		zend_throw_exception(php_imagickpixeliterator_exception_class_entry, description, (long)severity TSRMLS_CC); \
		IMAGICK_FREE_MEMORY(char *, description); \
		PixelClearIteratorException(pixel_iterator); \
		RETURN_NULL(); \
	} \
} \

/* Borrowed from zip extension */
#define IMAGICK_REGISTER_CONST_LONG(const_name, value)\
	zend_declare_class_constant_long(php_imagick_sc_entry, const_name, sizeof(const_name)-1, (long)value TSRMLS_CC);

#define IMAGICK_REGISTER_CONST_STRING(const_name, value)\
	zend_declare_class_constant_string(php_imagick_sc_entry, const_name, sizeof(const_name)-1, value TSRMLS_CC);

#define IMAGICK_CHECK_NOT_EMPTY(magick_wand, type, code)\
	if(MagickGetNumberImages(magick_wand) == 0) { \
		IMAGICK_THROW_EXCEPTION_WITH_MESSAGE(IMAGICK_CLASS, "Can not process empty Imagick object", (long)code); \
	} \

#define IMAGICK_INITIALIZE_ZERO_ARGS(wand_type, intern)\
	intern = (wand_type)zend_object_store_get_object(getThis() TSRMLS_CC);

#define IMAGICK_FREE_MEMORY(type, value)\
	if (value != (type) NULL) { \
		value = (type) MagickRelinquishMemory(value); \
		value = (type)NULL; \
	} \

#define IMAGICK_SAFE_MODE_CHECK(filename, status)\
	if (filename) { \
		if (strlen(filename) > MAXPATHLEN) { \
			status = IMAGICK_READ_WRITE_FILENAME_TOO_LONG; \
		} \
		if (PG(safe_mode) && (!php_checkuid_ex(filename, NULL, CHECKUID_CHECK_FILE_AND_DIR, CHECKUID_NO_ERRORS))) { \
			status = IMAGICK_READ_WRITE_SAFE_MODE_ERROR; \
		} \
		if (php_check_open_basedir_ex(filename, 0 TSRMLS_CC)) { \
			status = IMAGICK_READ_WRITE_OPEN_BASEDIR_ERROR; \
		} \
	} \

#define IMAGICK_HAS_FORMAT(buffer, magick_wand, free_buffer)\
	buffer = MagickGetImageFormat(magick_wand);\
	if(buffer == (char *)NULL || *buffer == '\0') {\
		IMAGICK_FREE_MEMORY(char *, buffer); IMAGICK_THROW_EXCEPTION_WITH_MESSAGE(IMAGICK_CLASS, "Image has no format", 1); \
	} else { \
		if (free_buffer) IMAGICK_FREE_MEMORY(char *, buffer); \
	} \

#define IMAGICK_METHOD_DEPRECATED(class_name, method_name) \
	php_error(E_STRICT, "%s::%s method is deprecated and it's use should be avoided", class_name, method_name);

#define IMAGICK_METHOD_DEPRECATED_USE_INSTEAD(class_name, method_name, new_class, new_method) \
	php_error(E_STRICT, "%s::%s is deprecated. %s::%s should be used instead", class_name, method_name, new_class, new_method);

#define IMAGICK_CHECK_READ_OR_WRITE_ERROR(intern, filename, error, free, message) \
	switch (error) { \
		case IMAGICK_READ_WRITE_NO_ERROR: \
			/* No error */ \
		break; \
		case IMAGICK_READ_WRITE_SAFE_MODE_ERROR:\
			zend_throw_exception_ex(php_imagick_exception_class_entry, 1 TSRMLS_CC, "Safe mode restricts user to read image: %s", filename); \
			if (free == IMAGICK_FREE_FILENAME) { \
				 efree(filename); \
			} \
			RETURN_NULL();\
		break; \
		case IMAGICK_READ_WRITE_OPEN_BASEDIR_ERROR: \
			zend_throw_exception_ex(php_imagick_exception_class_entry, 1 TSRMLS_CC, "open_basedir restriction in effect. File(%s) is not within the allowed path(s)", filename); \
			if (free == IMAGICK_FREE_FILENAME) { \
				 efree(filename); \
			} \
			RETURN_NULL(); \
		break; \
		default: \
		case IMAGICK_READ_WRITE_UNDERLYING_LIBRARY: \
			{ \
				ExceptionType severity; \
				char *description = MagickGetException(intern->magick_wand, &severity); \
				if (strlen(description) == 0) { \
					zend_throw_exception_ex(php_imagick_exception_class_entry, 1 TSRMLS_CC, message, filename);\
				} else {\
					zend_throw_exception(php_imagick_exception_class_entry, description, 1 TSRMLS_CC);\
					IMAGICK_FREE_MEMORY(char *, description);\
					MagickClearException(intern->magick_wand);\
					description = (char *)NULL;\
				}\
			}\
			if (free == IMAGICK_FREE_FILENAME) { \
				 efree(filename); \
			} \
			RETURN_NULL();\
		case IMAGICK_READ_WRITE_PERMISSION_DENIED:\
			zend_throw_exception_ex(php_imagick_exception_class_entry, 1 TSRMLS_CC, "Permission denied to: %s", filename);\
			if (free == IMAGICK_FREE_FILENAME) { \
				 efree(filename); \
			}\
			RETURN_NULL();\
		break;\
		case IMAGICK_READ_WRITE_FILENAME_TOO_LONG:\
			zend_throw_exception_ex(php_imagick_exception_class_entry, 1 TSRMLS_CC, "Filename too long: %s", filename);\
			if (free == IMAGICK_FREE_FILENAME) { \
				 efree(filename); \
			}\
			RETURN_NULL();\
		break;\
		case IMAGICK_READ_WRITE_PATH_DOES_NOT_EXIST:\
			zend_throw_exception_ex(php_imagick_exception_class_entry, 1 TSRMLS_CC, "The path does not exist: %s", filename);\
			if (free == IMAGICK_FREE_FILENAME) { \
				 efree(filename); \
			}\
			RETURN_NULL();\
		break;\
	} \

#define IMAGICKDRAW_CHECK_READ_OR_WRITE_ERROR(internd, filename, error, free)\
	switch (error) {\
		default:\
		case 0:\
			/* No error */\
		break;\
		case 1:\
			zend_throw_exception_ex(php_imagickdraw_exception_class_entry, 1 TSRMLS_CC, "Safe mode restricts user to read file: %s", filename);\
			if (free == IMAGICK_FREE_FILENAME) { \
				 efree(filename); \
			}\
			RETURN_NULL();\
		break;\
		case 2:\
			zend_throw_exception_ex(php_imagickdraw_exception_class_entry, 1 TSRMLS_CC, "open_basedir restriction in effect. File(%s) is not within the allowed path(s)", filename);\
			if (free == IMAGICK_FREE_FILENAME) { \
				 efree(filename); \
			}\
			RETURN_NULL();\
		break;\
		case 3:\
			IMAGICK_THROW_IMAGICKDRAW_EXCEPTION(internd->drawing_wand, "Unable to read file", 1);\
			if (free == IMAGICK_FREE_FILENAME) { \
				 efree(filename); \
			}\
			RETURN_NULL();\
		break;\
	} \

#define IMAGICK_CORRECT_ITERATOR_POSITION(intern)\
	MagickSetLastIterator(intern->magick_wand);

#define IMAGICK_REPLACE_MAGICKWAND(intern, new_wand)\
	if (intern->magick_wand == (MagickWand *)NULL) {\
		intern->magick_wand = new_wand; \
	} else { \
		intern->magick_wand = (MagickWand *)DestroyMagickWand(intern->magick_wand); \
		intern->magick_wand = new_wand; \
	}

#define IMAGICKPIXEL_REPLACE_PIXELWAND(intern, new_wand)\
	if(intern->pixel_wand != (PixelWand *)NULL && intern->initialized_via_iterator != 1) {\
		intern->pixel_wand = (PixelWand *)DestroyPixelWand(intern->pixel_wand);\
		intern->pixel_wand = new_wand;\
	} else {\
		intern->pixel_wand = new_wand;\
	} \

#define IMAGICKDRAW_REPLACE_DRAWINGWAND(intern, new_wand) \
	if (intern->drawing_wand == (DrawingWand *)NULL) { \
		intern->drawing_wand = new_wand; \
	} else { \
		intern->drawing_wand = (DrawingWand *)DestroyDrawingWand(intern->drawing_wand); \
		intern->drawing_wand = new_wand; \
	} \

#define IMAGICK_CAST_PARAMETER_TO_COLOR(param, internp, caller) \
	switch (Z_TYPE_P(param)) { \
		case IS_STRING: \
		{ \
			zval *object; \
			PixelWand *pixel_wand = NewPixelWand(); \
			if (!PixelSetColor(pixel_wand, Z_STRVAL_P(param))) { \
				pixel_wand = DestroyPixelWand(pixel_wand); \
				IMAGICK_THROW_EXCEPTION_WITH_MESSAGE(caller, "Unrecognized color string", caller); \
				return; \
			} \
			MAKE_STD_ZVAL(object); \
			object_init_ex(object, php_imagickpixel_sc_entry); \
			internp = (php_imagickpixel_object *)zend_object_store_get_object(object TSRMLS_CC); \
			internp->initialized_via_iterator = 0; \
			efree(object); \
			IMAGICKPIXEL_REPLACE_PIXELWAND(internp, pixel_wand); \
		} \
		break; \
		case IS_OBJECT: \
			if (instanceof_function_ex(Z_OBJCE_P(param), php_imagickpixel_sc_entry, 0 TSRMLS_CC)) { \
				internp = (php_imagickpixel_object *)zend_object_store_get_object(param TSRMLS_CC); \
			} else { \
				IMAGICK_THROW_EXCEPTION_WITH_MESSAGE(caller, "The parameter must be an instance of ImagickPixel or a string", (long)caller); \
			} \
		break; \
		default: \
			IMAGICK_THROW_EXCEPTION_WITH_MESSAGE(caller, "Invalid parameter provided", (long)caller); \
		break; \
	} \

#define IMAGICK_CAST_PARAMETER_TO_OPACITY(param, internp, caller) \
	switch (Z_TYPE_P(param)) { \
		case IS_LONG: \
		case IS_DOUBLE: \
		{ \
			zval *object; \
			PixelWand *pixel_wand = NewPixelWand(); \
			PixelSetOpacity(pixel_wand, Z_DVAL_P(param)); \
			MAKE_STD_ZVAL(object); \
			object_init_ex(object, php_imagickpixel_sc_entry); \
			internp = (php_imagickpixel_object *)zend_object_store_get_object(object TSRMLS_CC); \
			internp->initialized_via_iterator = 0; \
			efree(object); \
			IMAGICKPIXEL_REPLACE_PIXELWAND(internp, pixel_wand); \
		} \
		break; \
		case IS_OBJECT: \
			if (instanceof_function_ex(Z_OBJCE_P(param), php_imagickpixel_sc_entry, 0 TSRMLS_CC)) { \
				internp = (php_imagickpixel_object *)zend_object_store_get_object(param TSRMLS_CC); \
			} else { \
				IMAGICK_THROW_EXCEPTION_WITH_MESSAGE(caller, "The parameter must be an instance of ImagickPixel or a string", (long)caller); \
			} \
		break; \
		default: \
			IMAGICK_THROW_EXCEPTION_WITH_MESSAGE(caller, "Invalid parameter provided", (long)caller); \
		break; \
	} \

#define IMAGICK_CALCULATE_THUMBNAIL_SIDES(magick_wand, desired_width, desired_height, fit) \
{ \
	long orig_width = MagickGetImageWidth(magick_wand);\
	long orig_height = MagickGetImageHeight(magick_wand);\
	double ratio;\
	if (fit) { \
		if ((desired_width <= 0) || (desired_height <= 0)) { \
			IMAGICK_THROW_EXCEPTION_WITH_MESSAGE(IMAGICK_CLASS, "Invalid image geometry", 1); \
		} \
		if ((orig_width <= desired_width) && (orig_height <= desired_height)) { \
			RETURN_TRUE; \
		} \
		if ((orig_width - desired_width) > (orig_height - desired_height)) { \
			ratio = (double)orig_width / (double)desired_width; \
			desired_height = (double)orig_height / ratio; \
		} else { \
			ratio = (double)orig_height / (double)desired_height; \
			desired_width = (double)orig_width / ratio; \
		} \
		if (desired_width < 1) { \
			desired_width = 1; \
		} \
		if (desired_height < 1) { \
			desired_height = 1; \
		} \
	} else { \
		if ((desired_width <= 0) && (desired_height <= 0)) { \
			IMAGICK_THROW_EXCEPTION_WITH_MESSAGE(IMAGICK_CLASS, "Invalid image geometry", 1); \
		} \
		if (desired_width <= 0) { \
			ratio = (double)orig_height / (double)desired_height; \
			desired_width = orig_width / ratio; \
		} else if (desired_height <= 0) { \
			ratio = (double)orig_width / (double)desired_width; \
			desired_height = orig_height / ratio; \
		} \
	}\
} \

#ifdef HAVE_LOCALE_H
#define IMAGICK_SET_LOCALE(old_locale, tmp, restore) \
	if ((IMAGICK_G(locale_fix)) && \
		(tmp = setlocale(LC_NUMERIC, NULL)) != NULL && \
		(strcmp(tmp, IMAGICK_LC_NUMERIC_LOCALE) != 0)) { \
		old_locale = estrdup(tmp); \
		setlocale(LC_NUMERIC, IMAGICK_LC_NUMERIC_LOCALE); \
		restore = 1; \
	} else { \
		restore = 0; \
	} \

#define IMAGICK_RESTORE_LOCALE(old_locale, restore) \
	if (restore && old_locale != NULL && (strcmp(old_locale, IMAGICK_LC_NUMERIC_LOCALE) != 0)) { \
 		setlocale(LC_NUMERIC, old_locale); \
		efree(old_locale); \
	} \

#else
#define IMAGICK_SET_LOCALE(old_locale, tmp, restore)
#define IMAGICK_RESTORE_LOCALE(old_locale, restore)
#endif

#if MagickLibVersion >= 0x635
#define IMAGICK_CLONE_PIXELWAND(source, target) \
	target = ClonePixelWand(source);
#else
#define IMAGICK_CLONE_PIXELWAND(source, target) \
	target = (PixelWand *)NewPixelWand(); \
	PixelSetColorCount(target, PixelGetColorCount(source)); \
	PixelSetRed(target, PixelGetRed(source)); \
	PixelSetGreen(target, PixelGetGreen(source)); \
	PixelSetBlue(target, PixelGetBlue(source)); \
	PixelSetOpacity(target, PixelGetOpacity(source)); \
	PixelSetAlpha(target, PixelGetAlpha(source));
#endif

#endif /* PHP_IMAGICK_MACROS_H */
